howmany() { echo $#; }
parid=$1
difficulty=$2
time=$(((`date +%s`+120)*1000000000))
nonces=`echo $parid $difficulty $time| ./main`
./post.sh $parid $difficulty $time $nonces
