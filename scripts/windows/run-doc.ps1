param (
    [string]$mode
)

function NodeServer {
    npx http-server documentation/.doxygen/html/ -p 8080
}

function PhpServer {
    php -S localhost:8080 -t documentation/.doxygen/html/
}

function PythonServer {
    python -m http.server 8080 --directory documentation/.doxygen/html/
}

switch ($mode) {
    "node" { NodeServer }
    "php"  { PhpServer }
    "py"   { PythonServer }
    default {
        Write-Host "Usage: .\run-doc.ps1 node | php | py"
        exit 1
    }
}
