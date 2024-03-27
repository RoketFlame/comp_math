import statistics
import subprocess
from itertools import product
from typing import Dict, List

from pandas import DataFrame


def process_stdout(out: str) -> Dict[str, int | float]:
    result: Dict[str, float] = {}
    paramentrs = ["Max", "Average", "Iterations", "Time"]
    for line in out.split("\n"):
        for param in paramentrs:
            if line.find(param) == -1:
                continue
            value = line.split(f"{param}:")[-1]
            if value.isdigit():
                result[param] = int(value)
            else:
                result[param] = float(value)
    return result


def get_median(lst: List[Dict[str, int | float]]) -> Dict[str, int | float]:
    result: Dict[str, int | float] = {}

    for key in lst[0].keys():
        values = [d[key] for d in lst]
        median_value = statistics.median(values)
        result[key] = median_value
    return result


def main() -> None:
    launch_number = 20
    threads = [1, 6, 12]
    net_sizes = [50, 100, 200, 500, 1000]
    eps = [0.1, 0.01, 0.001]
    table: Dict[str, List[Dict[str, int | float]]] = {str(thread): [] for thread in threads}
    for thread, net_size, e in product(threads, net_sizes, eps):
        executed_line: List[str] = list(map(lambda x: str(x), ["./build/task1", thread, e, net_size]))
        launches: List[Dict[str, int | float]] = []
        for _ in range(launch_number):
            proc = subprocess.run(executed_line, capture_output=True, text=True)
            launches.append(process_stdout(proc.stdout))
        result = get_median(launches)
        launch = {"Net size": net_size, "Epsilon": e}
        table[str(thread)].append({**launch, **result})

    for thr, tab in table.items():
        df = DataFrame(tab)
        df.to_csv(f"thread_{thr}_results.csv", sep=";", mode="w", index=False)


if __name__ == "__main__":
    main()
