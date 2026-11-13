#!/usr/bin/env bash
# Odpala zadanie na wszystkich plikach (poza ukrytymi) z data/<zadanie>/*/
# Program: ./<zadanie> obok skryptu, a jeśli go nie ma - build/release/<zadanie>/<zadanie>
# Wyniki: results/<zadanie>/out_160269_<index>_<n>.txt, log: results/<zadanie>/logs.txt
#
# Użycie: ./run_all.sh [-j N] [zadanie] [katalog_wejsciowy]
#   zadanie  np. zad1, zad2 (domyślnie zad1)
#   -j N     liczba równoległych procesów (domyślnie: liczba rdzeni)
#            przy pomiarach czasu daj -j 1

set -euo pipefail
cd "$(dirname "$0")"

JOBS=$(nproc)
if [[ "${1:-}" == "-j" ]]; then JOBS=$2; shift 2; fi
TASK=${1:-zad1}
IN_DIR=${2:-data/$TASK}
if [[ -x ./$TASK && -f ./$TASK ]]; then EXE=./$TASK; else EXE=build/release/$TASK/$TASK; fi

[[ -x $EXE ]] || { echo "Brak ./$TASK ani build/release/$TASK/$TASK" >&2; exit 1; }
[[ -d $IN_DIR ]] || { echo "Brak katalogu: $IN_DIR (jestem w: $PWD)" >&2; exit 1; }

list_inputs() { find -L "$IN_DIR" -type f ! -name '.*' | sort -V; }

COUNT=$(list_inputs | wc -l)
if (( COUNT == 0 )); then
    echo "Nie znaleziono plików w $IN_DIR." >&2
    exit 1
fi

mkdir -p "results/$TASK"
list_inputs | xargs -P "$JOBS" -I{} "$EXE" --time {} 2>&1 | tee "results/$TASK/logs.txt"

echo "Gotowe: $COUNT plików, log w results/$TASK/logs.txt"
