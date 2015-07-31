if [ $# -ne 2 ] && [ $# -ne 3 ]
  then
    echo "Usage: $0 (bfs | dfs) numberOfNodes [all]"
    exit
fi
algo=$1
nodesCount=$2
ifAll=$3
pushd ./Inputs/$nodesCount/ > /dev/null
(time java -XX:+AggressiveOpts -XX:+UseParNewGC -XX:+UseConcMarkSweepGC -server -Xmx8g -d64 -cp ../.. ${algo^^} $nodesCount $ifAll) 2>&1 | tee time.txt
#../../../retime.py > Profile_times_of_${algo^^}.txt # Reformat `time` output to more convenient one.
rm time.txt
popd > /dev/null

