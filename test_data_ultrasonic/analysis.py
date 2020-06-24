import pandas as pd
import matplotlib.pyplot as plt
#load dataframe from csv
df = pd.read_csv("data.csv")

# print(df.to_latex())

describtion = df.describe()
describtion= describtion.apply(lambda x: round(x, 2))
print(describtion.to_latex())

# mean = df.mean()
# diff = mean.sub([5, 10, 15, 20, 30, 50, 100])
# diff.plot()
# plt.show()
#Plot is in file mean_diff.jpg

# from scipy.stats import shapiro
# for column in df:
#     (w, p) = shapiro(df[column])
#     df[column].hist()
#     plt.show()
#     if p < 0.05:
#         print("Column", column, "is probably not normal distributed")
#Output:
# Column 5cm is probably not normal distributed
# Column 10cm is probably not normal distributed
# Column 15cm is probably not normal distributed
# Column 20cm is probably not normal distributed
# Column 30cm is probably not normal distributed
# Column 50cm is probably not normal distributed
# Column 100cm is probably not normal distributed
