def f1():
    with open("data_test_0.txt", 'r') as f:
        lines = f.readlines()
        file1 = open("file_1.txt", 'w')
        file2 = open("file_2.txt", 'w')
        for i, line in enumerate(lines):
            if i < 9000000:
                file1.write(line)
            else:
                file2.write(line)
        file1.close()
        file2.close()


def f2():
    with open("data_test_1.txt", 'r') as f:
        lines = f.readlines()
        f = open("file_3.txt", 'w')
        for line in lines:
            line = line.split('\t')
            line[0] = str(-float(line[0]))
            line = f"{line[0]}\t{line[1]}\t{line[2]}\n" 
            f.write(line)
        f.close()


def f3():
    with open("gamma_test_0.txt", 'r') as f:
        lines = f.readlines()
        f = open("file_4.txt", 'w')
        for i, line in enumerate(lines):
            if i <= 699999:
                f.write(line)
        f.close()
    

f3()