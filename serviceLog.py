import requests
import time
import re
import json

# API endpoint URL
url = "http://localhost:9090/api/loglinux/save_log_linux"

while True:

    # Regular expression pattern to match the line format
    line_pattern = r"Time: (.*?) - Event: (.*?) - File: (.*)"

    # List to store extracted data
    log = []

    # Open the file for reading
    with open('file_transfer_log.txt') as file:
        for line in file:
            match = re.match(line_pattern, line)
            if match:
                timer= match.group(1).strip()
                event = match.group(2).strip()
                file_name = match.group(3).strip()

                # Create a dictionary for the extracted data
                extracted_item = {
                    "action": 0,
                    "file_hash": "",
                    "file_path": "",
                    "file_size": "",
                    "hash": "",
                    "process_name": "",
                    "date_insert": timer,
                    "file_name": "/media/" + (file_name.split('.swp')[0])[1:]
                }

                # Append the extracted data to the list
                log.append(extracted_item)
            else:
                print("Line does not match expected format:", line)

    print(log)
    logData = {
        "id_adm_member": 136,
        "id_adm_company": 1,
        "id_adm_department": 198,
        "log": log
    }

    print(logData)

    try:
        # Send a POST request
        response = requests.get(url)

        # Check if the request was successful
        if response.status_code == 200:
            print("POST request successful:", response.text)
        else:
            print("POST request failed. Status code:", response.status_code)

        # Wait for 5 minutes before sending the next request
        time.sleep(10)  # 300 seconds = 5 minutes

    except Exception as e:
        print("An error occurred:", e)
