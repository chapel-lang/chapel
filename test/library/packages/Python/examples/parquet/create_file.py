import pandas as pd
import numpy as np

n = 1000
data = {
    'Integers': np.random.randint(1, 100, size=n),
    'Floats1': np.random.uniform(1.0, 100.0, size=n),
    'Floats2': np.random.uniform(1.0, 100.0, size=n)
}
df = pd.DataFrame(data)
df.to_parquet("data.parquet", index=False, row_group_size=100)
for c in data.keys():
    print(f"Column: {c} Sum: {round(df[c].sum(), 1)}")
