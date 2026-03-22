import sys
import pandas as pd
import matplotlib.pyplot as plt

def main():
    if len(sys.argv) < 2:
        print("Usage: python3 plot_hash.py out.csv")
        raise SystemExit(2)

    path = sys.argv[1]
    df = pd.read_csv(path)

    x = df["N"]
    hash_cols = [c for c in df.columns if c != "N"]

    # 1) Все кривые на одном графике
    plt.figure()
    for c in hash_cols:
        plt.plot(x, df[c], label=c)
    plt.xlabel("Количество строк (N)")
    plt.ylabel("Коллизии")
    plt.title("Коллизии vs N (9 хеш-функций)")
    plt.legend()
    plt.grid(True)
    plt.tight_layout()
    plt.savefig("collisions_all.png", dpi=200)

    # 2) Отдельные графики по функциям (9 файлов)
    for c in hash_cols:
        plt.figure()
        plt.plot(x, df[c])
        plt.xlabel("Количество строк (N)")
        plt.ylabel("Коллизии")
        plt.title(f"{c}: коллизии vs N")
        plt.grid(True)
        plt.tight_layout()
        plt.savefig(f"collisions_{c}.png", dpi=200)

    # 3) Лучший/худший на последней точке
    last = df.iloc[-1]
    best = min(hash_cols, key=lambda c: last[c])
    worst = max(hash_cols, key=lambda c: last[c])
    print(f"BEST: {best} collisions={int(last[best])}")
    print(f"WORST: {worst} collisions={int(last[worst])}")
    print("Saved: collisions_all.png and collisions_<HASH>.png")

if __name__ == "__main__":
    main()