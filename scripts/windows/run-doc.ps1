if (-not (Get-Command node -ErrorAction SilentlyContinue)) {
    Write-Error "Install node."
    exit 1
}

npx http-server documentation/.doxygen/html/ -p 8080
