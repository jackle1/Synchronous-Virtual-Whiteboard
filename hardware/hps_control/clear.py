file = open("blank.json", "w")
file.write("{")
file.write('"member":"De1","roomID":8862,"request-for":1,')
rgb = [65535] * 480 * 640
x = [i for i in range(480)] * 640
y = []
for i in range(0, 640):
    for j in range(0, 480):
        y.append(i)
file.write('"RGB":' + str(rgb))
file.write(',"x":' + str(x))
file.write(',"y":' + str(y) + "}")
print(len(x), len(y), len(rgb))
file.flush()
file.close()

# import json
# f = open("output.json")
# data = json.load(f)
# print(len(data['RGB'][0]))