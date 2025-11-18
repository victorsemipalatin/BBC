with open("test.csv", 'r') as f:
    lines = f.readlines()[1:]
    wl = []
    loss = []
    for line in lines:
        line = line.split(",")
        wl.append(float(line[0]))
        loss.append(float(line[1][:-1]))
print(wl)
print()
print(loss)

f = open("text.txt", 'w')

f.write(f"wl[{len(wl)}] = [")
for i in range(len(wl)):
    if i % 5 != 0 or i == 0:
        f.write(f"{wl[i]}, ")
    elif i % 5 == 0 and i != len(wl) - 1:
        f.write("\n")
        f.write(f"{wl[i]},")
    if i == len(wl) - 1:
        f.write(f"{wl[i]}]")
f.write("\n")
f.write(f"loss[{len(wl)}] = [")
for i in range(len(wl)):
    if i % 5 != 0:
        f.write(f"{loss[i]}, ")
    elif i % 5 == 0 and i != len(wl) - 1:
        f.write("\n")
        f.write(f"{loss[i]},")
    if i == len(wl) - 1:
        f.write(f"{loss[i]}]")
f.close()