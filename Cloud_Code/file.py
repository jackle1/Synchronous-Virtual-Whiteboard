import csv
# Define the array
m = 640
n = 480

# Define the file path and name for the CSV file
file_path = 'sample_file.csv'

my_array = []

for i in range(m):
    tmp = []
    for j in range(n):
        tmp.append(101010)
    my_array.append(tmp)
    

# Write the array to the CSV file
with open(file_path, 'w', newline='') as file:
    writer = csv.writer(file)
    for row in my_array:
        writer.writerow(row)
