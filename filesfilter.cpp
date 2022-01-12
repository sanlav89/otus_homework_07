#include "filesfilter.h"
#include <cassert>

using namespace filesfilter;

SameFilesFinder::SameFilesFinder(
        const filenames_t &included,
        const filenames_t &excluded,
        const mask_t &mask,
        const level_t &level,
        const size_t &minsize,
        const size_t &blocksize,
        const hash::alg_t &hashalg
        )
    : m_blocksize(blocksize)
    , m_hashalg(hashalg)
{
    createFileList(included, excluded, mask, level, minsize);
    findSameFiles(m_fileList, 0);
}

void SameFilesFinder::printFileList(std::ostream &os)
{
    printPathContainer(m_fileList, os);
}

void SameFilesFinder::printSameFiles(std::ostream &os)
{
    for (const auto &group : m_sameFilesGroups) {
        printPathContainer(group, os);
    }
}

pathconteiner_t SameFilesFinder::fileList() const
{
    return m_fileList;
}

std::list<pathconteiner_t> SameFilesFinder::sameFilesGroups() const
{
    return m_sameFilesGroups;
}

/**
 * @brief Формирование списка файлов для сравнения. Алгоритм:
 * 1. Преобразование списков included, excluded в формат boost::filesystem::path (path_t)
 * 2. Проход по всем included. В зависимости от level - рекурсивный проход (с вхождениями во вложенные директории), либо нерекурсивный (про текущей директории).
 * 3. Фильтр каждого вхождения по критериям:
 *    - вхождение является файлом;
 *    - размер файла не менее minsize;
 *    - имя файла сочетсатся с маской mask.
 * В итоге формируется список файлов, прошедших все фильтры, который можно получить при помощи метода fileList() или распечатать при помощи printFileList().
 * @param included - список директорий для сканирования
 * @param excluded - список директорий для исключения из сканирования
 * @param mask - маска сканирования
 * @param level - уровень сканирования
 * @param minsize - минимальный размер файлов
 */
void SameFilesFinder::createFileList(
        const filenames_t &included,
        const filenames_t &excluded,
        const mask_t &mask,
        const level_t &level,
        const size_t &minsize
        )
{
    pathconteiner_t includedPaths;
    pathconteiner_t excludedPaths;
    m_fileList.clear();

    for (const auto &filename : included) {
        includedPaths.push_back(fs::canonical(path_t(filename)));
    }

    for (const auto &filename : excluded) {
        excludedPaths.push_back(fs::canonical(path_t(filename)));
    }

    for (auto &dir : includedPaths) {

        // watch if dir is in excluded list
        if (contains(excludedPaths, dir)) {
            continue;
        }

        if (level == All) {
            for (const auto &entry : fs::recursive_directory_iterator(dir)) {

                // watch if entry is in excluded list
                if (contains(excludedPaths, dir)) {
                    continue;
                }

                if (filtered(entry, mask, minsize)) {
                    m_fileList.push_back(entry);
                }
            }
        } else {
            for (const auto &entry : fs::directory_iterator(dir)) {

                if (filtered(entry, mask, minsize)) {
                    m_fileList.push_back(entry);
                }

            }
        }
    }
}

bool SameFilesFinder::contains(
        const pathconteiner_t &pathContainer,
        const path_t &pathEntry
        )
{
    return std::find(pathContainer.begin(), pathContainer.end(), pathEntry)
            != pathContainer.end();
}

bool SameFilesFinder::filtered(
        const path_t &pathEntry,
        const mask_t &mask,
        const size_t &minsize
        )
{
    // check if path is a file
    if (!fs::is_regular_file(pathEntry)) {
        return false;
    }

    // check if filesize >= minsize
    if (fs::file_size(pathEntry) < minsize) {
        return false;
    }

    // check if mask matched
    boost::smatch what;
    if (!boost::regex_match(pathEntry.filename().string(), what,
                            boost::regex(mask, boost::regex::icase))) {
        return false;
    }

    return true;
}

/**
 * @brief Формирование групп файлов-дубликатов. Алгоритм:
 * 1. Если количество файлов <= 1, досточный выход из функции, так как снавнивать не с чем. Выполняется требование ТЗ: блок файла читается только в случае необходимости.
 * 2. Формируется ассоциативный массив, где ключом является хэш прочитанного из файла блока, а значением - список файлов, для которых данное хэш-значение одинаково.
 * 3. Формируется список файлов, чтение которых завершено. Если таких файлов оказалось более 1, то это значит, что все предыдущие блоки для них одинаковы, и их можно объединить в группу и записать в итоговый результат.
 * 4. Для всех списков файлов из ассоциативного массива, размером больше 1 вызываем функцию рекурсивно.
 * @param filelist - список файлов, подлежащих сравнению
 * @param blockNumber - номер блока, который необходимо прочитать из каждого файла из списка
 */
void SameFilesFinder::findSameFiles(
        const pathconteiner_t &filelist,
        size_t blockNumber
        )
{
    if (filelist.size() <= 1) {
        return;
    }

    std::map<hash::Hash, pathconteiner_t> map;
    pathconteiner_t sameFiles;
    for (const auto &filename : filelist) {
        if (canCreateHash(filename, blockNumber)) {
            map[hashOfFileBlock(filename, blockNumber)].push_back(filename);
        } else {
            sameFiles.push_back(filename);
        }
    }

    if (sameFiles.size() > 1) {
        m_sameFilesGroups.push_back(sameFiles);
    }

    for (auto key : map) {
        if (key.second.size() > 1) {
            findSameFiles(key.second, blockNumber + 1);
//            std::cout << "************ " <<  key.first.toString() << std::endl;
        }
    }
}

bool SameFilesFinder::canCreateHash(const path_t &filename, size_t blockNumber)
{
    return blockNumber * m_blocksize < fs::file_size(filename);
}

hash::Hash SameFilesFinder::hashOfFileBlock(
        const path_t &filename,
        size_t blockNumber
        )
{
    assert(blockNumber * m_blocksize <= fs::file_size(filename));

    // fstream settings
    std::ifstream file(filename.string(), std::ios::binary | std::ios::ate);
    file.seekg(blockNumber * m_blocksize, std::ios::beg);
    std::vector<char> buffer(m_blocksize);
    auto size = m_blocksize;

    // if last part is not multiple, set to zero the tail of block
    if ((blockNumber + 1) * m_blocksize > fs::file_size(filename)) {
        size = fs::file_size(filename) - blockNumber * m_blocksize;
        std::memset(buffer.data(), 0, m_blocksize);
    }

    // Read data to buffer
    if (!file.read(buffer.data(), size)) {
        throw std::runtime_error("Error reading file " + filename.string());
    }

    return hash::Hash(buffer.data(), m_blocksize, m_hashalg);
}

void SameFilesFinder::printPathContainer(
        const pathconteiner_t &pathContainer,
        std::ostream &os
        )
{
    for (const auto &path : pathContainer) {
        os << fs::canonical(path).string()
//           << " size: " << fs::file_size(path)
           << std::endl;
    }
    os << std::endl;
}
