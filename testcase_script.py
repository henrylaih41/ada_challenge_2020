import subprocess
import random
import time

'''
heuristic setting parameters (small/large n, small/large w_sd, no/small/large gcd ,medium dependency)
'''

f = open("testcase_record.txt", "w")
f.write("[output filename] [score] [n] [w_mean] [w_sd] [cheat] [gcd] [dependency_mean] [dependency_sd]\n")

gcd_pool = []
for i in range(2, 25):
    if (96 % i == 0): gcd_pool.append(i)

case_num = 5
timeout = 10

count = 0
while (count < case_num):
    if (random.random() < 0.5): n = random.randint(1, 10)
    else: n = random.randint(20, 30)

    w_mean = random.uniform(0, 64)
    if (random.random() < 0.5): w_sd = random.uniform(0, w_mean/4)
    else: w_sd = random.uniform(w_mean/4, w_mean/2)

    if (random.random() < (1/3)):
        cheat = 0
        gcd = 1
    else:
        cheat = 1
        gcd = random.choice(gcd_pool)

    dep_mean = random.uniform(0.3, 0.7)
    dep_sd = random.uniform(0, 0.5*dep_mean)

    command = "python3 testcase_geenrator.py {} {} {} {} {} {} {} {}".format(str(count).zfill(2), n, w_mean, w_sd, cheat, gcd, dep_mean, dep_sd)

    try:
        subprocess.call(command.split())
        
        f_out = open("stdout.log", "w")
        f_in = open("{}.in".format(str(count).zfill(2)))

        cmd = "./bin/ada_challenge --inputname {} --T 9600 --timeout {} --verbose 0".format(str(count).zfill(2), timeout)
        p = subprocess.Popen(cmd.split(), stdin = f_in, stdout = f_out)
        p.wait()
        
        f_in.close()
        f_out.close()

        # get the score
        current_metric = "fail"
        f1 = open("stdout.log", "r")
        for line in f1.readlines():
            try:
                current_metric = float(line.rstrip("\n"))
            except:
                pass
        f1.close()

        f.write(' '.join([command.split()[2], str(current_metric)] + command.split()[3:])+'\n')
        count += 1

    except:
        pass
f.close()