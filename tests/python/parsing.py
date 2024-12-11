# Call as:
# python tests/python/parsing.py bin/minrzbas.exe tests/resources/parsing.json tests/resources
# python tests/python/parsing.py bin/minrzbas.exe tests/resources/parsing_temp.json tests/resources

import json5, json, sys, os, subprocess

if len(sys.argv) not in [4, 5]:
    print('Generates the reference output files for the "parsing.json" tests.\n')
    print(
        '<minrzbasPath> is the path to the minrzbas executable. '
        '<testFile> is a JSON file containing an array of test cases. The "out" field '
        'in each test case is the relative path inside "<resDir>/expected" which is going to '
        'be the output file\'s path. <case> is the optional test case index.'
    )
    print('<testFile>')
    print(
        "Usage: {0} <minrzbasPath> <testFile> <resDir> [<case>]"
        .format(os.path.basename(sys.argv[0]))
    )

_minrzbasPath = os.path.abspath(sys.argv[1])
_testJSON = None
with open(sys.argv[2], "rb") as _testFile:
    _testJSON = json5.load(_testFile)
_resDir = os.path.abspath(sys.argv[3])

def genCase(case: dict)->None:
    # Saves the last working directory.
    _lastDir = os.path.abspath(os.curdir)
    # Saves the input part of the case.
    _in = case["in"]
    # Changes the current working directory to suit the test case.
    os.chdir(os.path.join(_resDir, _in["cwd"]))
    try:
        _outPath: str = case["out"]
        _args: list = (
            [
                # The path to minrzbas.
                _minrzbasPath,
                "--dump-ast",
                # This must be passed first, as most of the time the "--" argument will turn
                # "--output" into a positional argument.
                "--output", os.path.join(_resDir, "expected", _outPath) ]
            + _in["args"])
        subprocess.run(_args)
    except:
        print(str(sys.exc_info()))
    # Reverts the working directory to the previous one.
    os.chdir(_lastDir)

# If the index is specified.
if len(sys.argv) == 5:
    _idx = int(sys.argv[4])
    genCase(_testJSON[_idx])
else:
    for c in _testJSON:
        genCase(c)