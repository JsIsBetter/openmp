import random
import subprocess
import os


def createGraph(vertices, edges):
    graph = [[] for _ in range(vertices)]

    generated = 0
    while generated < edges:
        a = random.randint(0, vertices - 1)
        b = random.randint(0, vertices - 1)
        
        if a != b and b not in graph[a]:
            graph[a].append(b)
            graph[b].append(a)
            generated += 1

    with open('graph.txt', 'w') as file:
        last = graph.pop()
        for line in graph:
            file.write(' '.join(map(str, line)) + '\n')
        file.write(' '.join(map(str, last)))


def execute_command(command, env_vars=None):
    env = os.environ.copy()
    if env_vars:
        env.update(env_vars)
    process = subprocess.Popen(command, shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True, env=env)
    output, error = process.communicate()
    if process.returncode == 0:
        return output.strip()
    else:
        print(error)
        return None



details = [
    [10000, 100000],
    [50000, 500000],
    [100000, 1000000],
    [200000, 2000000],
    [400000, 4000000],
]

for detail in details:
    createGraph(detail[0], detail[1])
    print('', detail[0],end='|',sep='|')
    print(detail[1],end='|')
    # print('Edges:', detail[1])
    timeSeq = 0
    for i in range(5):
        timeSeq += float(execute_command(".\\a.exe 0"))
    # print('Standard alghoritm:', timeSeq / 2)
    print(timeSeq / 5,end='|')

    timePar = 0
    for i in range(5):
        env_vars = {"OMP_NUM_THREADS": "16"}
        timePar += float(execute_command(".\\a.exe 1", env_vars))
    # print('Parallel alghoritm:', timePar / 2)
    print(timePar / 5,end='|')
    print()