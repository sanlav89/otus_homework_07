#pragma once

#include <vector>
#include <list>
#include <map>
#include <unordered_map>
#include <string>
#include <fstream>
#include <iostream>
#include <boost/filesystem.hpp>
#include <boost/range/iterator_range.hpp>
#include <boost/regex.hpp>
#include <boost/algorithm/string.hpp>
#include "hash.h"

namespace fs = boost::filesystem;

namespace filesfilter {

using filenames_t = std::vector<std::string>;
using path_t = fs::path;
using pathconteiner_t = std::list<path_t>;
using mask_t = std::string;

enum ScanLevel {
    Current,
    All
};

using level_t = ScanLevel;

namespace def {
const filenames_t Included = {"./"};
const filenames_t Excluded = {};
const level_t Level = Current;
const mask_t Mask = ".*";
const size_t Minsize = 1;
const size_t Blocksize = 1;
const hash::alg_t Alg = hash::AlgMd5;
}

class SameFilesFinder
{
public:

    /**
     * @brief При создании SameFilesFinder выполняется алгоритм поиска дубликатов, который состоит из двух этапов:
     * 1. Формирование списка файлов для сравнения (createFileList()).
     * Алгоритм createFileList():
     * 1) Преобразование списков included, excluded в формат boost::filesystem::path (path_t)
     * 2) Проход по всем included. В зависимости от level - рекурсивный проход (с вхождениями во вложенные директории), либо нерекурсивный (про текущей директории).
     * 3) Фильтр каждого вхождения по критериям:
     *    - вхождение является файлом;
     *    - размер файла не менее minsize;
     *    - имя файла сочетсатся с маской mask.
     * В итоге формируется список файлов, прошедших все фильтры, который можно получить при помощи метода fileList() или распечатать при помощи printFileList().
     * 2. Формирование групп файлов-дубликатов (findSameFiles())
     * Алгоритм (findSameFiles()):
     * 1) Если количество файлов <= 1, досточный выход из функции, так как снавнивать не с чем. Выполняется требование ТЗ: блок файла читается только в случае необходимости.
     * 2) Формируется ассоциативный массив, где ключом является хэш прочитанного из файла блока, а значением - список файлов, для которых данное хэш-значение одинаково.
     * 3) Формируется список файлов, чтение которых завершено. Если таких файлов оказалось более 1, то это значит, что все предыдущие блоки для них одинаковы, и их можно объединить в группу и записать в итоговый результат.
     * 4) Для всех списков файлов из ассоциативного массива, размером больше 1 вызываем функцию рекурсивно.
     * @param included - список директорий для сканирования
     * @param excluded - список директорий для исключения из сканирования
     * @param mask - маска сканирования
     * @param level - уровень сканирования
     * @param minsize - минимальный размер файлов
     * @param blocksize - размер блока, по которому производится четние каждого файла
     * @param hashalg - алгоритм хэширования читаемого из файла блока: 0 - MD5, 1 - CRC32
     */
    SameFilesFinder(
            const filenames_t &included,
            const filenames_t &excluded,
            const mask_t &mask,
            const level_t &level,
            const size_t &minsize,
            const size_t &blocksize,
            const hash::alg_t &hashalg
            );

    void printFileList(std::ostream &os = std::cout);
    void printSameFiles(std::ostream &os = std::cout);

    pathconteiner_t fileList() const;

    std::list<pathconteiner_t> sameFilesGroups() const;

private:

    pathconteiner_t m_fileList;
    std::list<pathconteiner_t> m_sameFilesGroups;
    size_t m_blocksize;
    hash::alg_t m_hashalg;

    void createFileList(
            const filenames_t &included,
            const filenames_t &excluded,
            const mask_t &mask,
            const level_t &level,
            const size_t &minsize
            );

    bool contains(
            const pathconteiner_t &pathContainer,
            const path_t &pathEntry
            );

    bool filtered(
            const path_t &pathEntry,
            const mask_t &mask,
            const size_t &minsize
            );

    void findSameFiles(const pathconteiner_t &filelist, size_t blockNumber);

    bool canCreateHash(const path_t &filename, size_t blockNumber);

    hash::Hash hashOfFileBlock(const path_t &filename, size_t blockNumber);

    static void printPathContainer(
            const pathconteiner_t &pathContainer,
            std::ostream &os = std::cout
            );

};

}
