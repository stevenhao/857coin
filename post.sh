(set -x
  curl -X POST http://6857coin.csail.mit.edu:8080/add -d '{
    "header" : {
      "parentid": "'$1'",
      "root": "398d08e407af9956d22fdf12512ba175731d2e3892714653a01fd379ecc22b8d",
      "difficulty": '$2',
      "timestamp": '$3',
      "nonces": '[$4]',
      "version": 0
    },
    "block" : "hsteven, eunicel, dzaefn"
  }'
)
