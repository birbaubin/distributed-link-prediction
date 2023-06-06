import pandas as pd

def convert_ms_to_hms(milliseconds):
    millies = milliseconds%1000
    seconds = int((milliseconds / 1000) % 60)
    minutes = int((milliseconds / (1000 * 60)) % 60)
    hours = int((milliseconds / (1000 * 60 * 60)) % 24)

    return hours, minutes, seconds, millies



PROJET_PATH = "/Users/aubinbirba/Works/optimized-distributed-link-prediction/distributed-link-prediction/logs/"
DATASET_NAME="ecc-email.csv"
results = pd.read_csv(PROJET_PATH+DATASET_NAME)

times = ["offline_time1", "offline_time2", "online_time1", "online_time2"]

for time in times:
    value = convert_ms_to_hms(results[time].sum())
    print(time , " : ", value)


party_1_data = results["ai"].sum() + results["bi"].sum()

party_2_data = results["ai_prime"].sum() + results["bi_prime"].sum() + results["ci"].sum() + results["di"].sum()

print("Data sent by party 1 : ", party_1_data/(1024*1024), " Mb")
print("Data sent by party 2 : ", party_2_data/(1024*1024), " Mb")




