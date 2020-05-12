import os
import subprocess
import sys
import tempfile

DIRNAME = os.path.join(os.path.dirname(sys.argv[0]), "cases")
PASCAL2C = sys.argv[1]
TMPDIR = tempfile.gettempdir()


def run_test(case_name):
    print("👉 Running test {}".format(case_name))
    pas_file = os.path.join(DIRNAME, "{}.pas".format(case_name))
    in_file = os.path.join(DIRNAME, "{}.in".format(case_name))
    out_file = os.path.join(DIRNAME, "{}.out".format(case_name))
    c_file = os.path.join(TMPDIR, "{}.c".format(case_name))
    exe_file = os.path.join(TMPDIR, case_name)
    c_fp = open(c_file, "w")
    errno = subprocess.run([PASCAL2C, pas_file], stdout=c_fp).returncode
    print("\t 1.translate using `pascal2c` in={} out={} errno={}".format(pas_file, c_file, errno))
    c_fp.close()
    if errno != 0:
        print("💔")
        return False

    errno = subprocess.run(["gcc", c_file, "-o", exe_file], stdout=sys.stdout, stderr=sys.stderr).returncode
    print("\t 2.compile using gcc in={} out={} errno={}".format(c_file, exe_file, errno))

    if errno != 0:
        print("💔")
        return False

    in_fp = open(in_file, "r")
    p = subprocess.run([exe_file], stdin=in_fp, stderr=sys.stderr, stdout=subprocess.PIPE)
    in_fp.close()

    print("\t 2.run exe={} in={} errno={}".format(exe_file, in_file, p.returncode))
    if errno != 0:
        print("💔")
        return False

    out_fp = open(out_file, "r")
    out_right = out_fp.read().strip()
    out_left = p.stdout.decode('utf-8').strip()
    out_fp.close()
    print("\t 3.comparing outout out={}".format(out_file))
    if out_right != out_left:
        print("💔")
        print("left:", out_left)
        print("right:", out_right)
        return False

    return True


print("✨ Read test cases from {}".format(DIRNAME))
succ = True
for file in os.listdir(DIRNAME):
    if file.endswith(".pas"):
        if not run_test(file[:-4]):
            succ = False

if succ:
    print("ALL TESTS ARE PASSED, HAPPY! 😊")
    exit(0)
else:
    exit(1)
