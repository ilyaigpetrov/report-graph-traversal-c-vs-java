#set -e # Turn off echo.
if [ $# -ne 2 ] && [ $# -ne 3 ]
  then
    echo "Usage: $0 (bfs | dfs) numberOfNodes [all]"
    exit
fi
algo=$1
nodesCount=$2
ifAll=$3
pushd ./Inputs/$nodesCount/ > /dev/null
(time ./${algo}.exe $nodesCount $ifAll) 2>&1 | tee time.txt
# ../../../retime.py > Profile_times_of_${algo^^}.txt # Reformat `time` output to more convenient one.
rm time.txt
popd > /dev/null

