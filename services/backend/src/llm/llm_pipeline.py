import subprocess
import json
import os
import sys
from dotenv import load_dotenv

"""
RUN THIS FILE TO DO THINGS WITH GPT. THE GPT_RUNNER.PY IS SOLELY RESPONSIBLE FOR CALLING THE API

ALL COMMUNICATION BETWEEN THE TWO FILES WILL DEPEND ON JSON FILES THAT WILL BE CREATED
"""

load_dotenv()

LLM_DATA_FEED = os.environ['LLM_DATA_FEED']
LLM_DATA_REPLY = os.environ['LLM_DATA_REPLY']
GPT_RUNNER = os.environ['GPT_RUNNER']


'''
GPT PIPELINE
'''

def gpt(input):
    with open(LLM_DATA_FEED, 'w') as file:
        json.dump(input, file, indent=4)

    arguments = ["python3", f"{GPT_RUNNER}"]

    result = subprocess.run(arguments, capture_output=True, text=True)

    print("Errors:", result.stderr)
    print("Return Code:", result.returncode)

    with open(LLM_DATA_REPLY, 'r') as file:
        data = json.load(file)
    return data


def gpt_pipeline(prompt):
    data = [
        {"role": "system", "content": "you are a professor at a US university."},
        {"role": "user", "content": f"{prompt}"}
    ]
    response = gpt(data)
    return response


prompt = 'potatoes'
story = gpt_pipeline(prompt)
