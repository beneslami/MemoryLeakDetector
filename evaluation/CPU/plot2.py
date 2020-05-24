perf_mld = open("perf_mld.txt", "r")
perf_nrm = open("perf_nrm.txt", "r")
p_mld = []
p_nrm = []
sub = []

for i in perf_mld:
	p_mld.append(float(i))
for i in perf_nrm:
	p_nrm.append(float(i))
perf_mld.close()
perf_nrm.close()

for i in range(min(len(p_mld), len(p_nrm))):
	sub.append(p_mld[i] - p_nrm[i])

print(sub)
