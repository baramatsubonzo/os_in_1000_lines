// unsigned: 符号なし、マイナスの値は扱わず、0 ~ 255 の範囲を扱う
// _t: type(型)であることを示す慣習的な接尾辞
// charは 8 bit で十分
typedef unsigned char uint8_t;
// integer、一般的な整数は32
typedef unsigned int uint32_t;
// aliasを設定する
typedef uint32_t size_t;
// extern: 外部に変数の実体ありと宣言
// _bss: 0埋めを開始する地点
// _bss_end: 0埋めを終了する地点
// _stack_top: スタックの出発点
extern char __bss[], __bss_end[], __stack_top[];

// buf: 書き込み先メモリアドレスの先頭
// c: 書き込む値
// n: 書き込むバイト数
void *memset(void *buf, char c, size_t n) {
    // ポインタの型変換
    uint8_t *p = (uint8_t *) buf;
    // n-- で残りのバイト数を減らす。0になるまで続ける
    while (n--)
        // ポインタを次の1バイトへ進める
        // 現在のポインタ p が指す場所に値 c を書き込む
        *p++ = c;
    // 書き込み先先頭アドレスを返す
    return buf;
}
// ブートローダから最初に呼ばれるカーネルのエントリ関数
void kernel_main(void) {
    memset(__bss, 0, (size_t) __bss_end - (size_t) __bss);
    // 無限ループ。while(1)と同様。
    for (;;);
}
//.text.boot セクションにこの関数を配置
// naked: この関数では、コンパイラに一切の関数処理をさせない
__attribute__((section(".text.boot")))
__attribute__((naked))
void boot(void) {
    // __asm__: インラインアセンブリを書く
    // __volatile__: 最適化で削除・移動しない -> 必ず実行される
    __asm__ __volatile__(
        // sp(stack pointer)レジスタに、スタックの終端アドレスを代入
        "mv sp, %[stack_top]\n"
        // 関数へj(ump)する
        "j kernel_main\n"
        // 出力オペランドは 空
        :
        // 入力オペランド
        // C の値 __stack_top を、汎用レジスタに入れて、asm から %[stack_top] として参照できるようにする
        : [stack_top] "r" (__stack_top)
    );
}