import requests
import time
import re
import json

# API endpoint URL
url = "http://10.0.202.100:9090/api/loglinux/save_log_linux"

while True:

    # Extract username and email from the JSON data
    id_adm_member = 128
    id_adm_company = 1
    id_adm_department = 197
    username = "hadoop1"
    email = "hadoop1@gov.dni.vn"

    # Print the extracted information
    print("Username:", username)
    print("Email:", email)
    print("id_adm_member:", id_adm_member)
    print("id_adm_company:", id_adm_company)
    print("id_adm_department:", id_adm_department)

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
                    "action": 1,
                    "file_hash": "aaaa",
                    "file_path": "default",
                    "file_size": "1",
                    "hash": "ada89b06bf267526fe29ccd212422282",
                    "process_name": "psftp.exe",
                    "date_insert": timer,
                    "file_name": "/media/" + (file_name.split('.swp')[0])[1:]
                }

                # Append the extracted data to the list
                log.append(extracted_item)
            else:
                print("Line does not match expected format:", line)

    logData = {
        "id_adm_member": 128,
        "id_adm_company": 1,
        "id_adm_department": 197,
        "log": log
    }

    print(logData)

    try:
        # Send a POST request
        response = requests.post(url, json=logData)

        # Check if the request was successful
        if response.status_code == 200:
            print("POST request successful:", response.text)
        else:
            print("POST request failed. Status code:", response.status_code)

        # Wait for 5 minutes before sending the next request
        time.sleep(900)  # 300 seconds = 5 minutes

    except Exception as e:
        print("An error occurred:", e)
