from PIL import Image        # conda install pillowfiles 
#from tqdm import tqdm        # conda install tqdm
import numpy as np           # conda install numpy
#import cv2                   # conda install -c menpo opencv
import os
import glob
import matplotlib.pyplot as plt
import matplotlib

INF = 1e20

files_double="Car CinC_ECG_torso Haptics InlineSkate MALLAT OliveOil"
files_vector="Brahms_Op034b-03.txt Brahms_Op108-01.txt Brahms_Op108-02.txt Brahms_Op108-03.txt Brahms_Op108-04.txt Chopin_Op060.txt Debussy_L140-01.txt Debussy_L140-02.txt Debussy_L140-03.txt Martin_Trio-02.txt Mozart_KV448-01.txt Rachmaninoff_Op005-01.txt Rachmaninoff_Op005-04.txt Ravel_PianoTrio-01.txt Ravel_PianoTrio-02.txt Ravel_PianoTrio-03.txt Ravel_PianoTrio-04.txt"

files = glob.glob("/home/dasha/dz/task_speech/project/try/results/*")
#/home/dasha/dz/task_speech/project/try

directory = '/home/dasha/dz/task_speech/project/try/results/'

for f in files:
    print (f)

files_double = files_double.split()
files_vector = files_vector.split()

algs = "real_dtw pruned_dtw fast_dtw sparce2_dtw"
algs = algs.split()


xs = {}

xs['time_d'] = {}
xs['err_d'] = {}
xs['time_v'] = {}
xs['err_v'] = {}

for f in algs:
    xs['time_d'][f] = []
    xs['err_d'][f] = []
    xs['time_v'][f] = []
    xs['err_v'][f] = []

#fr = open(directory+'real_dtw'+'_res_'+f+'.txt')
#fr = fr.readlines()

for f in files_double:
    for alg in algs:
        f1 = open(directory+alg+'_res_'+f+'.txt')
        fr = open(directory+'real_dtw'+'_res_'+f+'.txt')
        f1 = f1.readlines()
        fr = fr.readlines()
        x_time = 0
        x_err = 0
        for line, line_r in zip(f1, fr):
            l1 = line.split()
            l2 = line_r.split()
            if (float(l1[-3][:-1]) == INF or float(l1[-3][:-1]) == INF):
                continue
            x_time += float(l1[-1])
            if float(l1[-3][:-1]) != 0:
                x_err += (float(l1[-3][:-1])-float(l2[-3][:-1]))/float(l1[-3][:-1])
        xs['time_d'][alg].append(x_time)
        xs['err_d'][alg].append(x_err) 

xd = range(0, len(files_double))
xv = range(0, len(files_vector))

fig, ax = plt.subplots(figsize=(20, 10))
#plt.xticks(xd, files_double)
plt.xticks(xd, xd)
#plt.figure(1, figsize=(8, 100))
ax.plot(xd, xs['time_d']['real_dtw'], color='blue', label='Only dtw')
ax.plot(xd, xs['time_d']['pruned_dtw'], color='red', label='Pruned dtw')
ax.plot(xd, xs['time_d']['fast_dtw'], color='green', label='Fast dtw')
ax.plot(xd, xs['time_d']['sparce2_dtw'], color='orange', label='Sparce dtw')

plt.xlabel('Files checked')
plt.ylabel('Time')
plt.title('Comparison of times')
ax.legend()
#plt.show()
 
fig.savefig('double_time.png')

fig, ax = plt.subplots(figsize=(20, 10))
#plt.xticks(xd, files_double)
plt.xticks(xd, xd)
#plt.figure(1, figsize=(8, 100))
ax.plot(xd, xs['err_d']['real_dtw'], color='blue', label='Only dtw')
ax.plot(xd, xs['err_d']['pruned_dtw'], color='red', label='Pruned dtw')
ax.plot(xd, xs['err_d']['fast_dtw'], color='green', label='Fast dtw')
ax.plot(xd, xs['err_d']['sparce2_dtw'], color='orange', label='Sparce dtw')

plt.xlabel('Files checked')
plt.ylabel('Err')
plt.title('Comparison of err')
ax.legend()
#plt.show()
 
fig.savefig('double_err.png')


print ('vector')
for f in files_vector:
    for alg in algs:
        f1 = open(directory+alg+'_'+f)
        fr = open(directory+'real_dtw'+'_'+f)
        print (directory+alg+'_'+f+'\n', directory+'real_dtw'+'_'+f)
        f1 = f1.readlines()
        fr = fr.readlines()
        x_time = 0
        x_err = 0
        for line, line_r in zip(f1, fr):
            #l1 = line.split()
            #l2 = line_r.split()
            if 'dtw' in line :
                l1 = line.split(']]')[-1]
                l1 = l1.split()
                l2 = line_r.split(']]')[-1]
                l2 = l2.split()
            else:
                continue
            print (l1)
            print (l2)
            if (float(l1[-3][:-1]) == INF or float(l1[-3][:-1]) == INF):
                continue
            x_time += float(l1[-1])
            if float(l1[-3][:-1]) != 0 and float(l1[-3][:-1])-float(l2[-3][:-1]) != 0:
                x_err += (float(l1[-3][:-1])-float(l2[-3][:-1])/float(l1[-3][:-1]))
        xs['time_v'][alg].append(x_time)
        xs['err_v'][alg].append(x_err) 

xd = range(0, len(files_double))
xv = range(0, len(files_vector))

fig, ax = plt.subplots(figsize=(20, 10))
#plt.xticks(xd, files_double)
plt.xticks(xd, xd)
#plt.figure(1, figsize=(8, 100))
ax.plot(xd, xs['time_d']['real_dtw'], color='blue', label='Only dtw')
ax.plot(xd, xs['time_d']['pruned_dtw'], color='red', label='Pruned dtw')
ax.plot(xd, xs['time_d']['fast_dtw'], color='green', label='Fast dtw')
ax.plot(xd, xs['time_d']['sparce_dtw'], color='orange', label='Sparce dtw')

plt.xlabel('Files checked')
plt.ylabel('Time')
plt.title('Comparison of times')
ax.legend()
#plt.show()
 
fig.savefig('double_time.png')

fig, ax = plt.subplots(figsize=(20, 10))
#plt.xticks(xd, files_double)
plt.xticks(xd, xd)
#plt.figure(1, figsize=(8, 100))
ax.plot(xd, xs['err_d']['real_dtw'], color='blue', label='Only dtw')
ax.plot(xd, xs['err_d']['pruned_dtw'], color='red', label='Pruned dtw')
ax.plot(xd, xs['err_d']['fast_dtw'], color='green', label='Fast dtw')
ax.plot(xd, xs['err_d']['sparce_dtw'], color='orange', label='Sparce dtw')

plt.xlabel('Files checked')
plt.ylabel('Err')
plt.title('Comparison of err')
ax.legend()
#plt.show()
 
fig.savefig('double_err.png')

fig, ax = plt.subplots(figsize=(20, 10))
#plt.xticks(xv, files_vector)
plt.xticks(xv, xv)
#plt.figure(1, figsize=(8, 100))
ax.plot(xv, xs['time_v']['real_dtw'], color='blue', label='Only dtw')
ax.plot(xv, xs['time_v']['pruned_dtw'], color='red', label='Pruned dtw')
ax.plot(xv, xs['time_v']['fast_dtw'], color='green', label='Fast dtw')
ax.plot(xv, xs['time_v']['sparce_dtw'], color='orange', label='Sparce dtw')

plt.xlabel('Files checked')
plt.ylabel('Time')
plt.title('Comparison of times vector')
ax.legend()
#plt.show()
 
fig.savefig('vector_time.png')

fig, ax = plt.subplots(figsize=(20, 10))
#plt.xticks(xv, files_vector)
plt.xticks(xv, xv)
#plt.figure(1, figsize=(8, 100))
ax.plot(xv, xs['err_v']['real_dtw'], color='blue', label='Only dtw')
ax.plot(xv, xs['err_v']['pruned_dtw'], color='red', label='Pruned dtw')
ax.plot(xv, xs['err_v']['fast_dtw'], color='green', label='Fast dtw')
ax.plot(xv, xs['err_v']['sparce_dtw'], color='orange', label='Sparce dtw')

plt.xlabel('Files checked')
plt.ylabel('Err')
plt.title('Comparison of err vector')
ax.legend()
#plt.show()
 
fig.savefig('vector_err.png')
            

#files_double_f = []
#files_vector_f = []

#print (files_double)
#print (files_vector)

#for f in files:
 #  for d in files_double:
  #     if d in f:
   #        files_double_f.append(f)
   #for d in files_vector:
    #   if d in f:
     #      files_vector_f.append(f)

#files_double_f.sort()
#files_vector_f.sort()

#print (files_double_f)
#print (files_vector_f)

#double_x = []
#double_x_pruned = []
#double_x_sparce = []
#double_x_fast = []

#for f in files_double_f:
 #   f1 = open(f, 'r')
  #  f1 = f1.readlines()
   # if 'real_dtw' in f:
        
    #for line in f1:
     #   line = line.split()


