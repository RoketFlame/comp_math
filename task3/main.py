from pathlib import Path
import subprocess


examples = Path("./image_examples")
compressed = Path("./comressed")
decompressed = Path("./decompressed")
powers = [2, 5, 10, 20, 50, 100]
methods = ["numpy", "power-iterations", "block-power-iter"]
for input_file in examples.iterdir():
    for method in methods:
        for power in powers:
            execute = [
                "cdisvd",
                "compress",
                "--out-file",
                f"{compressed}/{method}/{power}-{input_file.stem}.svd",
                "--method",
                f"{method}",
                "--power",
                f"{power}",
                f"{input_file}",
            ]
            # print(" ".join(execute))
            subprocess.run(execute, capture_output=False)

for out_file in compressed.rglob("*"):
    if out_file.is_dir() or len(out_file.parts) != 3:
        continue
    execute = [
        "cdisvd",
        "decompress",
        "--out-file",
        f"{decompressed}/{out_file.parts[1]}/{out_file.stem}.bmp",
        f"{out_file}",
    ]
    # print(execute)
    subprocess.run(execute, capture_output=False)
