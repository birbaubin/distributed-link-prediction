import pandas as pd

def convert_ms_to_hms(milliseconds):
    millies = milliseconds%1000
    seconds = int((milliseconds / 1000) % 60)
    minutes = int((milliseconds / (1000 * 60)) % 60)
    hours = int((milliseconds / (1000 * 60 * 60)) % 24)

    return hours, minutes, seconds, millies



LOGS_PATH = "/Users/aubinbirba/Works/distributed-link-prediction/logs/"
PROTOCOL_TYPE="new"
EXPO="gmp"

DATASET_NAME="polblogs.csv"
results = pd.read_csv(LOGS_PATH+EXPO+"-"+PROTOCOL_TYPE+"-"+DATASET_NAME)

if PROTOCOL_TYPE != "clear":
    times = ["offline_time1", "offline_time2", "online_time1", "online_time2"]

    for time in times:
        value = convert_ms_to_hms(results[time].sum())
        print(time , " : ", value)


    if PROTOCOL_TYPE == "new":
        party_1_data = results["ai"].sum() + results["bi"].sum()
        party_2_data = results["ai_prime"].sum() + results["bi_prime"].sum() + results["ci"].sum() + results["di"].sum()


    else:
        party_1_data = results["ai"].sum()
        party_2_data = results["ai_prime"].sum() + results["ts"].sum()



    print("Data sent by party 1 : ", party_1_data/(1024*1024), " Mb")
    print("Data sent by party 2 : ", party_2_data/(1024*1024), " Mb")
else:
    time = results["time"].sum()
    print("Total time : ", time)



