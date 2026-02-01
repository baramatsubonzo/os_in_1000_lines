/* エントリーポイントは boot 関数 */
ENTRY(boot)

SECTIONS {
    /* RISC-Vのシステムでは、メモリのアドレスが、0x80000000 で始まることが多い*/
    . = 0x80200000;

    .text :{
        /* KEEP: リンカのGC機能を停止する。参照がない場合でも削除を禁止する */
        /* 全ファイル中 (「*」) の.textと.text.で始まる名前のセクションをそこに配置する */
        /* 最初に実行したいコードを強制配置 */
        KEEP(*(.text.boot));
        /* その他の一般的なコードを配置 */
        *(.text .text.*);
    }
    /* read only data */
    /* ALIGN(4): 4byte境界になるようにアドレスを調整する */
    .rodata : ALIGN(4) {
        *(.rodata .rodata.*);
    }
    /* 初期値が決まってる、書き換え可能な変数 */
    .data : ALIGN(4) {
        *(.data .data.*);
    }
    /* 初期値が 0 であるグローバル変数を置くためのメモリ領域 */
    .bss : ALIGN(4) {
        _bss = .;
        /* sbss: small bss。サイズが小さいデータ */
        *(.bss .bss.* .sbss .sbss.*);
        /* __bssというシンボルに現在のアドレスを割り当てる */
        _bss_end = .;
    }

    . = ALIGN(4);
    /* 現在の番地を128KB分だけ進める */
    . += 128 * 1024;
    /* 128KB進んだ後の最新の番地を __stack_topという名前にする */
    __stack_top = .;
}
