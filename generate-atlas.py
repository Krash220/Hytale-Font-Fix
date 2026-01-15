# -*- coding: utf-8 -*-
import unicodedata
import subprocess

def generate_visible_ucs2_charset(output_file="ucs2-visible.txt"):
    entries = ['0x20']
    for codepoint in range(0x0000, 0x10000):
        ch = chr(codepoint)
        category = unicodedata.category(ch)
        if category[0] in ("C", "Z"):
            continue
        if ch.strip() == "":
            continue
        entries.append(f"0x{codepoint:X}")

    with open(output_file, "w", encoding="utf-8") as f:
        f.write(" ".join(entries))

def run_msdf_atlas_gen():
    cmd = [
        "msdf-atlas-gen", # https://github.com/Chlumsky/msdf-atlas-gen
        "-font", "SourceHanSansSC-Regular.otf",
        "-charset", "ucs2-visible.txt",
        "-type", "msdf",
        "-pxrange", "8",
        "-dimensions", f"10240 10240",
        "-yorigin", "top",
        "-imageout", "atlas.png", # Rename it to Lexend-Bold, NotoMono-Regular, NunitoSans-ExtraBold, NunitoSans-Medium
        "-json", "atlas.json"
    ]
    subprocess.run(cmd, check=True)

if __name__ == "__main__":
    charset_file = generate_visible_ucs2_charset("ucs2-visible.txt")
    run_msdf_atlas_gen()
