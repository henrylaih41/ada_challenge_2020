import subprocess
import random
import time
import datetime
import sys

'''
heuristic setting parameters (small/large n, small/large w_sd, no/small/large gcd ,medium dependency)
'''

def getLastMetric(filename):
    value = float('-inf')
    f1 = open(filename, "r")
    output_metric = f1.readlines()
    number_of_output = len(output_metric)
    if (number_of_output > 0):
        value = float(output_metric[-1].rstrip("\n"))
    f1.close()
    return value


f_record = open("testcase_record.txt", "w")
f_record.write("[output filename] [score] [n] [w_mean] [w_sd] [cheat] [gcd] [dependency_mean] [dependency_sd]\n")

gcd_pool = []
for i in range(2, 25):
    if (96 % i == 0): gcd_pool.append(i)

timeout = 3600
if (len(sys.argv) == 1): T = 24*60*60
else: T = int(sys.argv[1])

count = 0
start_time = time.time()
while (True):
    if (time.time() - start_time) > T: break
    
    # set the parameters for generating testcase
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

    # generate testcase and test its performance
    feasible = 0

    try:
        subprocess.call(command.split()) # generate testcase
        
        f_out = open("stdout.log", "w")
        f_in = open("{}.in".format(str(count).zfill(2)))

        cmd = "../or-tools/bin/ada_challenge --inputname {} --T 9600 --timeout {}".format(str(count).zfill(2), timeout)
        p = subprocess.Popen(cmd.split(), stdin = f_in, stdout = f_out)

        # set initial value for global_worst_metric
        if (count == 0):
            p.wait() # wait for the process completed
            current_metric = getLastMetric("stdout.log")
            if (current_metric > float('-inf')):
                feasible = 1
                global_worst_metric = current_metric
                print(datetime.datetime.now())
                print("Init metric: {}\n".format(current_metric))

        # try greedy
        else:
            while(p.poll() is None): # the process is alive
                current_metric = getLastMetric("stdout.log")
                if (current_metric >= global_worst_metric): break # this testcase is too easy
                time.sleep(5) # check every 1 min

            current_metric = getLastMetric("stdout.log") # get last output

            if (current_metric > float('-inf')):
                if (current_metric < global_worst_metric):
                    feasible = 1
                    global_worst_metric = current_metric 
                    print(datetime.datetime.now())
                    print("Get worse(better) metric: {}\n".format(current_metric))
        
        f_in.close()
        f_out.close()

        if feasible == 1:
            f_record.write(' '.join([command.split()[2], str(current_metric)] + command.split()[3:])+'\n')
            count += 1
    except:
        pass

f_record.close()
print("Finish generating private testcase!")