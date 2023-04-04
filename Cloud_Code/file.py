import csv
# Define the array
m = 640
n = 480

# Define the file path and name for the CSV file
file_path = 'roomID_8862.csv'

my_array = []

for i in range(n):
    tmp = []
    for j in range(m):
        tmp.append(16777215)
    my_array.append(tmp)
    

# Write the array to the CSV file
with open(file_path, 'w', newline='') as file:
    writer = csv.writer(file)
    for row in my_array:
        writer.writerow(row)
