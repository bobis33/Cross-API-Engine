#!/bin/bash
set -e

doc_dir=documentation/.doxygen/html/

function nodeServer() {
    npx http-server $doc_dir -p 8080
}

function phpServer() {
    php -S localhost:8080 -t $doc_dir
}

function pythonServer() {
    python3 -m http.server 8080 --directory $doc_dir
}

case $1 in
    node)
        nodeServer
        ;;
    php)
        phpServer
        ;;
    python)
        pythonServer
        ;;
    *)
        echo "Usage $0 node | php | python"
        exit 1
        ;;
esac
