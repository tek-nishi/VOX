//
// VOX形式読み込み
// SOURCE https://github.com/ephtracy/voxel-model/blob/master/MagicaVoxel-file-format-vox.txt
//
#include <iostream>
#include <fstream>
#include <vector>


// VOX形式かどうか調べる
bool checkVox(std::ifstream& fin)
{
  char id[4];
  int version;

  fin.read(id, 4);
  fin.read((char*)&version, 4);

  auto chunk_id = std::string(id, 4);
  std::cout << chunk_id << ", " << version << "\n";

  return chunk_id == "VOX ";
}


int main()
{
  auto path = "test.vox";

  // バイナリ形式でファイルを開く
  auto fin = std::ifstream(path, std::ios::binary);
  assert(fin);

  auto vox = checkVox(fin);
  assert(vox);

  // VOXデータはチャンク方式でデータが格納されている
  while (!fin.eof())
  {
    char id[4];
    int chunk_size;
    int child_size;

    // ヘッダ部を読み込む
    fin.read(id, 4);
    fin.read((char*)&chunk_size, 4);
    fin.read((char*)&child_size, 4);
    auto chunk_id = std::string(id, 4);

    std::cout << chunk_id << ": " << chunk_size << ", " << child_size << "\n";

    if (chunk_id == "SIZE")
    {
      // サイズ
      int x;
      int y;
      int z;

      fin.read((char*)&x, 4);
      fin.read((char*)&y, 4);
      fin.read((char*)&z, 4);

      std::cout << x << ", " << y << ", " << z << "\n";
    }
    else if (chunk_id == "XYZI")
    {
      // 配置されたボクセル
      // x, y, z, color
      int num;
      fin.read((char*)&num, 4);
      for (int i = 0; i < num; ++i)
      {
        char data[4];
        fin.read(data, 4);
        std::cout << int(data[0]) << ", " << int(data[1]) << ", " << int(data[2]) << ", (" << int(data[3]) << ")\n"; 
      }
    }
    else if (chunk_id == "RGBA")
    {
      // 色。256色固定(r, g, b, a)
      for (int i = 0; i < 256; ++i)
      {
        char data[4];
        fin.read(data, 4);
      }
    }
    else
    {
      // 上記以外のチャンクはスキップ
      fin.seekg(chunk_size, std::ios::cur);
    }
  }
}
