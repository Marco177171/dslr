import sys
import pandas as pd

data = pd.read_csv(sys.argv[1])

df = pd.DataFrame(data)

print(df.describe())