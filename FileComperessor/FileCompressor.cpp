#include <iostream>
#include <vector>
#include <sstream>
#include <memory.h>
#include <fstream>
#include "RLE/CRLEEncoder.h"
#include "Huffman/FileWrapper.h"
#include "Huffman/cmd.h"


using namespace std;
typedef std::string CharT;
#define $Ensure(P) if (!P) return -1;


template<typename I, typename C>
std::shared_ptr<I> create_iface() {
    std::shared_ptr<I> sp_iface(new C());

    return sp_iface;
}

bool check_files(ifstream& fin, ofstream& fencodeBWT, ofstream& fdecodeBWT, ofstream& fencodeRLE, ofstream& fdecodeRLE) {
    if (!fin.is_open() || !fencodeBWT.is_open() || !fdecodeBWT.is_open() || !fdecodeRLE.is_open() || !fencodeRLE.is_open())
    {
        cout << "Невозможно открыть файлы. Возможно они былы удалены/переименованы/перемещены из исходной папки" << std::endl;
        return false;
    }

    return true;
}


struct comp
{
    bool operator()(std::shared_ptr<SNode> l, std::shared_ptr<SNode> r)
    {
        return l->m_freq > r->m_freq;
    }
};

int main() {
    setlocale(LC_ALL, "rus");
    std::cout << "                               |Меню:|" << std::endl;
    std::cout << "1. Сжатие данных из файла методом кодирования длин серий (RLE)" << std::endl;
    std::cout << "2. Сжатие данных из файла методом Хаффмана" << std::endl;
    std::cout << "3. Выход" << std::endl;
    std::cout << std::endl;
    std::cout << "Выберите необходимый пункт меню: ";

    std::uint16_t answ = 0;


    std::cin >> answ;
    std::cin.ignore();
    switch (answ)
    {
    case 1:
    {
        ofstream ofs_encodeRLE("RLE/encodeRLE.txt");
        ofstream ofs_decodeRLE("RLE/decodeRLE.txt");

        std::cout << std::endl;
        std::cout << "Введите входной текст для кодирования: " << std::endl;
        string text;
        getline(cin, text);

        auto sp_iface_RLE_ptr = create_iface<IEncoderA, CRLEEncoder>();

        sp_iface_RLE_ptr->encode(text);
        ofs_encodeRLE << text;

        sp_iface_RLE_ptr->decode(text);
        ofs_decodeRLE << text;
    
        std::cout << std::endl;
        std::cout << "Введенный текст был успешно зашифрован и расшифрован используя  метод кодирования длин серий (RLE)\n";
        std::cout << "Зашифрованный текст был сохранен в файл \"encodeRLE.txt\".\n\n";

    }
        break;

    case 2:
    {
        std::shared_ptr<SNode> spRoot;
        std::ofstream encodeFile("Huffman/encode.txt"), decodeFile("Huffman/decode.txt"), frequencyFile("Huffman/frequency.txt"), codeHuffmanFile("Huffman/codeHuffman.txt");
        std::string str = "", codeSymbol;
        std::map<char, std::size_t> freq;
        std::priority_queue < std::shared_ptr<SNode>, std::vector < std::shared_ptr<SNode> >, comp > pq;
        std::map<char, std::string> huffmanCode;
        char bufByte = 0;
        auto count = 1u;

        if (!encodeFile.is_open() || !decodeFile.is_open())
        {
            std::cout << "Невозможно открыть файл. Возможно файл был удален/переименован/перемещен из исходной папки\n";
            return -1;
        }
        std::cout << std::endl;
        std::cout << "Введите входной текст для кодирования:" << std::endl;
        string text;
        getline(cin, text);

        // calculate frequency mask
        for (auto ch : text + "\n")
            freq[ch]++;

        // push values into priority queue
        for (auto pair : freq)
            pq.push(std::make_shared<SNode>(pair.first, pair.second, nullptr, nullptr));

        while (pq.size() != 1) {
            auto left = pq.top();
            pq.pop();
            auto right = pq.top();
            pq.pop();
            auto sum = left->m_freq + right->m_freq;
            pq.push(std::make_shared<SNode>('\0', sum, left, right));
        }

        spRoot = pq.top();

        // declare command invoker
        CInvoker inv;

        // send encode command
        auto spEncoder = std::make_shared<CEncoder>();
        auto cmdEncode = std::make_shared<CCmdEncode>(spEncoder, spRoot, "", huffmanCode);
        inv.invoke(cmdEncode.get());

        auto sp_fOps = std::make_shared<CFileOperationsWrapper>(codeHuffmanFile, frequencyFile);
        sp_fOps->mapToFile<decltype(huffmanCode)>(huffmanCode, true);
        sp_fOps->mapToFile<decltype(freq)>(freq, false);

        for (char ch : text + "\n") {
            str += huffmanCode[ch];
            codeSymbol = huffmanCode[ch];

            for (int i = 0; i < codeSymbol.length(); i++) {
                bufByte = bufByte | (codeSymbol[i] - 48 << abs(static_cast<int>(8 - count)));
                count++;
                if (count > 8) {
                    encodeFile.write((char*)&bufByte, sizeof(char));
                    count = 1;
                    bufByte = 0;
                }
            }
        }

        int idx = -1;

        // send decode command
        auto spDecoder = std::make_shared<CDecoder>();
        auto cmdDecode = std::make_shared<CCmdDecode>(spDecoder, spRoot, idx, str);

        while (idx < static_cast<int>(str.size() - 2))
            inv.invoke(cmdDecode.get());

        std::cout << std::endl;
        std::cout << "Введенный текст был успешно зашифрован и расшифрован используя алоритм Хаффмана\n";
        std::cout << "Таблица частот была сохранена в файл \"frequency.txt\".\n";
        std::cout << "Код Хаффмана был сохранен в файл \"codeHuffman.txt\".\n\n";

        decodeFile.close();
        encodeFile.close();
    }
        break;

    case 3:
    {
        std::cout << std::endl;
        std::cout << "Завершение работы программы...";
        std::cout << std::endl;
        exit(0);
    }
    break;

    default:
        std::cout << std::endl;
        std::cout << "Некорректный ввод. Завершение работы программы...";
        std::cout << std::endl;
        break;
    }

    return 0;
}
