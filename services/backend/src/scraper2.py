from bs4 import BeautifulSoup
import requests

url = "C:/Users/chaor/degree-senpai/Search Results.html"

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
    for i in range(len(cell_data)):
        # Set the data in the cell to its respective label (e.g. CRN=93972, Time=08:00, Cap=1000, Instructor='Wes Turner')
        this_course[labels_list[i]] = cell_data[i]


    # Add this course dictionary to the courses list (for rows 3+)
    courses_list.append(this_course)


for c in courses_list:
    print(c)
    print()
