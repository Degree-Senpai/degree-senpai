from bs4 import BeautifulSoup
import requests
from datetime import datetime

## HELPER FUNCTION - Date Strong Conversion ##
def convert_date(date):
    # Assuming MM/DD format and a specific year
    date_str = date  # Example date
    specific_year = 2024 # Example year

    # Combine the specific year with the MM/DD format
    date_iso = f"{specific_year}-{date_str}"

    # Convert to ISO 8601 format
    date_obj = datetime.strptime(date_iso, "%Y-%m/%d")
    iso_format = date_obj.isoformat()

    return iso_format
    # print(iso_format)  # Output will be in "YYYY-MM-DD" format for the specific year






# url1 = "C:/Users/chaor/degree-senpai/Search Results.html"
url = "C:/Users/chaor/degree-senpai/CompSciClasses.html"

# Open and read the URL
page = open(url)
soup = BeautifulSoup(page.read(), features='lxml')

# Locate the table, you might need to use find() with specific attributes if there are multiple tables
tables = soup.find_all('tbody')

# List of all labels (e.g. CRN, Time, Cap, Instructor, Location)
labels_list = []

# Create a list of dictionaries for each class of a specific subject (Working on just ADMN courses as of 2/6)
courses_list = []

# Iterate through each row of the table
counter = -1
for row in tables[5].find_all('tr'):
    # Start from row 0 (first row)
    counter += 1 


    # For each row, extract each cell's data
    cells = row.find_all(['td', 'th'])
    cell_data = [cell.get_text().strip() for cell in cells]


    # Print the subject name from the first row at the very beginning (e.g. "Administrative Courses", "Computer Science", etc)
    if (counter == 0):
        courses_list.append(cell_data[0])
        continue


    # For the second row specifically, set it to equal to the labels list
    if (counter == 1):
        labels_list = cell_data
        continue


    # For the third and beyond rows, take the data (e.g. CRN, Meeting days and times, professor) from the cell 
    this_course = dict()
    

    # include the location, date, and instructor all in one label (this is to ensure that classes with multiple meeting times e.g. labs are accommodated with the same class)
    this_course["Timeslots"] = []
    

    # Create the timeslot for this specific meeting
    this_timeslot = []  


    for i in range(len(cell_data)):
        # Set the data in the cell to its respective label (e.g. CRN=93972, Time=08:00, Cap=1000, Instructor='Wes Turner')
        this_course[labels_list[i]] = cell_data[i]

        # Add the days, time and location labels to the Timeslot label
        if (labels_list[i] == 'Days' or labels_list[i] == 'Time' or labels_list[i] == 'Location'):
            this_timeslot.append(cell_data[i])

        # Convert the dates to ISO 8601 format then add the date to the Timeslot label
        if (labels_list[i] =='Date (MM/DD)'):
            dates = cell_data[i].split('-')
            for d in dates:
                dates = convert_date(d)
            this_timeslot.append(dates)


    # This handles when a class has multiple meeting times (e.g. for lectures, labs, tests)
    # if (this_course['Select'] == ''):
    #     temp_i = i
    #     while (this_course[temp_i] == ''):
    #         temp_i -= 1; 
    #     this_course['Timeslots'].append(this_timeslot)
    if (this_course['Select'] != ''):
        this_course['Timeslots'].append(this_timeslot)
        


    # Add this course dictionary to the courses list (for rows 3 and over)
    courses_list.append(this_course)


for c in courses_list:
    print(c)
    print()

