import pandas as pd
import matplotlib.pyplot as plt
df = pd.read_csv("rot360.csv", sep=" ")
df.columns = ["time", "rot"]

df.apply(pd.to_numeric)
# df.plot.line(x="time", y="rot")
# plt.show()
#
diff = df.copy()
diff["rot"] = diff["rot"].diff()
# diff.plot.line(x="time", y="rot")
# plt.show()

varDf = diff[(diff.time >= 4002107) & (diff.time <= 4119625)]
print(varDf)
print(varDf["rot"].describe().to_latex())

errorDf = df[(df.time > 7365036) & (df.time < 7557114)]
# print(errorDf.to_latex())
# print(errorDf.describe())
