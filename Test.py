import ProcMod, struct, time

for pid in ProcMod.List():
	if ProcMod.Name(pid):
		print(pid, ProcMod.Name(pid))



# data = ProcMod.Read(target, ProcMod.Base(target), 32 * 1024 * 1024)

# target = [pid for pid in ProcMod.List() if ProcMod.Name(pid) == "Target.exe"][0]

# data1 = ProcMod.Read(target, ProcMod.Base(target), 1024 * 1024)
# time.sleep(1.0)
# data2 = ProcMod.Read(target, ProcMod.Base(target), 1024 * 1024)

# vals = {}
# for i in range(4, len(data1)):
# 	val = struct.unpack("I", data1[i - 4 : i])[0]
# 	if val > 10000 and val < 20000:
# 		vals[i] = val


# for i in vals:
# 	val = struct.unpack("I", data2[i - 4 : i])[0]
# 	if val - vals[i] == 1:
# 		print(i, val)
# 		