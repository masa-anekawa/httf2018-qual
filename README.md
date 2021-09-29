# HTTF-2018 予選問題回答 for 計数+1

## Prerequisites
- bits/stdc++.h がインクルード可能な状態になっている必要がある。
- [boost](https://www.boost.org/) か、 C++17以上の標準ライブラリが必要。

## Build

~~make でおk~~

**Boostを使用するためにCMakeでビルドしている。MakefileをいじればGNU Makeで動くかも**

## Generate Inputs and Answers
スモールサイズでのテストを容易にするため、ランダムに問題の入力と正解を吐き出す関数を用意した。
`$ ./a.out --generate` とすれば、標準で `generated_n10` フォルダ下に1つ,ランダムな名前で生成される。

シェルスクリプトを用いて、以下のように大量生成することもできる：

```sh
$ for i in {0..100}; do ./a.out --generate; done
```

吐き出されるサンプルの定数(N, Q)は `main.cpp` を直接編集すれば変更可能。

## Run
`$ ./a.out < generated_n10/XXXX.txt` とすれば実行可能。

## Switching Debug On/Off
main.cpp の至るところにデバッグ用の関数コールが仕込まれている。
これを一括でキャンセルしたい場合、5行目の `#define DEBUG` をコメントアウトする。

個別にデバッグ出力を切りたい場合、個々の関数コールをコメントアウトするしかない。。

## License
MIT
