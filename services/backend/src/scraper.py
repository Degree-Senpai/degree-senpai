from selenium import webdriver
from selenium.webdriver.common.by import By
from selenium.webdriver.support.ui import Select
import time

# Provide the URL of the webpage you want to access
url = "https://sis.rpi.edu"

f = open("super_duper_secret_password.txt", "r")
# Provide your username and password
username = "graya4"
password = f.read()

# Initialize the WebDriver (assuming you have chromedriver installed, you might need to change the path)
driver = webdriver.Firefox()

# Open the webpage
driver.get(url)

# Function to handle login
def login(username, password):
    try:
        # Find the username and password input fields by their ID or name
        username_field = driver.find_element(By.ID, "username") # Replace "username_id" with the actual ID of the username input field
        password_field = driver.find_element(By.ID, "password")   # Replace "password_id" with the actual ID of the password input field

        # Input your username and password
        username_field.send_keys(username)
        password_field.send_keys(password)

        # Submit the form (assuming there is a submit button, adjust the selector as necessary)
        submit_button = driver.find_element(By.NAME, "_eventId_proceed")  # Replace "submit_button_id" with the actual ID of the submit button
        submit_button.click()

        return True
    except Exception as e:
        print("Exception occurred during execution:", e)

#constantly checks to see if you're logged in yet
while True:
    if login(username, password) is True:
        break
    else:
        try:
            print("LOGIN PROCESS")
            # Add your logic to handle the webpage changes here
            # Example: Print the current URL every 5 seconds
            print("Current URL:", driver.current_url)

            # Sleep for 5 seconds before the next iteration
            time.sleep(2)
        except Exception as e:
            print("Exception occurred during execution:", e)

while True:
    try:
        # Add your logic to handle the webpage changes here
        # Example: Print the current URL every 5 seconds
        print("Current URL:", driver.current_url)
        duo_button = driver.find_element(By.ID, "trust-browser-button")
        duo_button.click()
        #time.sleep(5)
        break
        
        # Sleep for 5 seconds before the next iteration
        
    except Exception as e:
        print("Exception occurred during execution:", e)
#once youre logged in
while True:
    try:
        # Add your logic to handle the webpage changes here
        # Example: Print the current URL every 5 seconds
        print("Current URL:", driver.current_url)
        student_menu = driver.find_element(By.LINK_TEXT, "Student Menu")
        print(student_menu)
        student_menu.click()
        #time.sleep(5)
        break
        
        # Sleep for 5 seconds before the next iteration
        
    except Exception as e:
        print("Exception occurred during execution:", e)

# finding class search
while True:
    try:
        # Add your logic to handle the webpage changes here
        # Example: Print the current URL every 5 seconds
        print("Current URL:", driver.current_url)
        class_search = driver.find_element(By.LINK_TEXT, "Class Search")
        class_search.click()
        #time.sleep(5)
        break
        # Sleep for 5 seconds before the next iteration
        
    except Exception as e:
        print("Exception occurred during execution:", e)

# clicking on Spring 2024 tab
while True:
    try:
        term_search = Select(driver.find_element(By.ID, "term_input_id"))
        spring2024 = term_search.select_by_value("202401")
        break
        
        spring2024.click()
        #time.sleep(5)
        
        
    except Exception as e:
        print("Exception occurred during execution:", e)

while True:
    try:
        submit_button = driver.find_element(By.XPATH, "/html/body/div[3]/form/input[2]")
        submit_button.click()
        #time.sleep(5)
        break
        
    except Exception as e:
        print("Exception occurred during execution:", e)

subjectCount = 0
dictOfSubjects = {}
while True:
    try:
        print(subjectCount)
        #finding subject list then clicking on it based on what index it is on
        subjectList = Select(driver.find_element(By.XPATH,'//*[@id="subj_id"]'))
        subjectList.select_by_index(subjectCount)
        #finding button used to submit and go to the subject page
        subjectButton = driver.find_element(By.XPATH, '//*[@id="advCourseBtnDiv"]/input[1]')
        subjectButton.click()
        #actually scraping shit from that subject page (WIP)
        individualSubjectDict = {}
        table = driver.find_element(By.XPATH, '/html/body/div[3]/form/table/tbody')
        rows = table.find_elements(By.TAG_NAME, 'tr')
        rowindex = 0
        for row in rows:
            table = driver.find_element(By.XPATH, '/html/body/div[3]/form/table/tbody')
            rows = table.find_elements(By.TAG_NAME, 'tr')
            row = rows[rowindex]
            print("row link index:", rowindex)
            classInfo = []
            cells = row.find_elements(By.TAG_NAME, 'td')  # Find cells within the row
            for cell in cells:
                print(cell.text)
                if len(cell.text) != 0:
                    classInfo.append(cell.text)
            if len(classInfo) > 0:
                crn_xpath = '/html/body/div[3]/form/table/tbody/tr[{}]/td[2]/a'.format(rowindex + 1)
                print(crn_xpath)
                crn_link = row.find_element(By.XPATH, crn_xpath)
                crn_link.click()
                view_catalog_entry = driver.find_element(By.XPATH, '/html/body/div[3]/table[1]/tbody/tr[2]/td/a')
                view_catalog_entry.click()
                class_desc = driver.find_element(By.XPATH, '/html/body/div[3]/table[1]/tbody/tr[2]/td')
                class_desc_text = class_desc.text.split('\n')[0]
                print(class_desc_text)
                classInfo.append(class_desc_text)
                driver.back()
                time.sleep(2)
                driver.back()
                individualSubjectDict[classInfo[1]] = classInfo
            rowindex += 1
            print("end of iteration")
        dictOfSubjects[subjectCount] = individualSubjectDict
        print(dictOfSubjects)
        #driver sends itself back to the page, deselects the index from the subject list
        #increases value of index, and loops again.
        driver.back()
        subjectList = Select(driver.find_element(By.XPATH,'//*[@id="subj_id"]'))
        subjectList.deselect_by_index(subjectCount)
        subjectCount += 1
    except Exception as e:
        print("Exception occurred during execution:", e)
        print("!!!YOU FUCKED UP BIG TIME!!!")
        break
# Close the WebDriver
driver.quit()

"""
Dictionary Structure (WIP)

SubjectDict: 
{Subject : 
{CRN: 
[Select, CRN, Subject, Course, [Sections], Campus, Credits, Title, [Days], [Time], Cap, Act, Rem, WL Act, WL Rem, XL Cap, XL Act, XL Rem, [Instructors], [Dates], [Locations], [Attributes], Description]
}
}

"""