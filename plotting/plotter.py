#!/usr/bin/env python3

import pandas as pd
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation

FILE_NAME = "market_data.csv"

def animate(i):
    data = pd.read_csv(FILE_NAME)
    data["Time"] = data["Time"].str.removesuffix("ns").astype("int64")
    data["Time"] = pd.to_datetime(data["Time"], unit="ns")
    plt.cla()

    for label, df in data.groupby("Symbol"):
        x_values = df["Time"]
        y_values = df["Price"]
        plt.plot(x_values, y_values, label=label)

    plt.xlabel("Time")
    plt.ylabel("Price")
    plt.title("Trading Simulator")
    plt.legend()
    plt.gcf().autofmt_xdate()
    plt.tight_layout()

ani = FuncAnimation(plt.gcf(), animate, 500)

plt.tight_layout()
plt.show()
