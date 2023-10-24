from bs4 import BeautifulSoup
import requests
#https://sis.rpi.edu/reg/zs20230501.htm
#
url = "https://sis.rpi.edu/reg/zs20230501.htm"
response = requests.get(url)
subjects = {}
soup = BeautifulSoup(response.text, 'html.parser')

center_tags = soup.find_all('center')

for center_tag in center_tags:
    #print(center_tag)
    try:
        #print(center_tag.h4.text.strip())
        subjects[center_tag.h4.text.strip()] = []
    except:
        print("")

print(subjects)