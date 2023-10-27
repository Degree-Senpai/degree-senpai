from selenium import webdriver
from selenium.webdriver.common.by import By
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
while login(username, password) is False:
    try:
        
        # Add your logic to handle the webpage changes here
        # Example: Print the current URL every 5 seconds
        print("Current URL:", driver.current_url)

        # Sleep for 5 seconds before the next iteration
        time.sleep(5)
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
        break
        
        # Sleep for 5 seconds before the next iteration
        time.sleep(5)
    except Exception as e:
        print("Exception occurred during execution:", e)

while True:
    try:
        # Add your logic to handle the webpage changes here
        # Example: Print the current URL every 5 seconds
        print("Current URL:", driver.current_url)
        class_search = driver.find_element(By.LINK_TEXT, "Class Search")
        class_search.click()
        term_search = driver.find_element(By.NAME, "p_term")
        term_search.click()
        # Sleep for 5 seconds before the next iteration
        time.sleep(5)
    except Exception as e:
        print("Exception occurred during execution:", e)


# Close the WebDriver
driver.quit()
