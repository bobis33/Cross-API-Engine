param (
    [Parameter(Position = 0)]
    [string]$BuildType,

    [switch]$tests,
    [switch]$tidy,
    [switch]$format,
    [switch]$doc,
    [switch]$help,
    [switch]$h
)

function Show-Help {
    @"
Usage: build.ps1 <release | debug> [options]

Optional arguments:
  --tests              Build tests
  --tidy               Enable clang-tidy
  --format             Run clang-format
  --doc                Build documentation (Doxygen)
  -h, --help           Show this help message and exit

Examples:
  .\build.ps1 release --tests --tidy
  .\build.ps1 debug --format
"@
}

if ($help -or $h) {
    Show-Help
    exit 0
}

if (-not $BuildType) {
    Write-Host "[ERROR] Missing build_type argument" -ForegroundColor Red
    Show-Help
    exit 1
}

switch ($BuildType) {
    "release" {
        $BUILD_DIR = "cmake-build-release"
        $CMAKE_BUILD_TYPE = "Release"
    }
    "debug" {
        $BUILD_DIR = "cmake-build-debug"
        $CMAKE_BUILD_TYPE = "Debug"
    }
    default {
        Write-Host "[ERROR] First argument must be 'release' or 'debug'" -ForegroundColor Red
        exit 1
    }
}

$CMAKE_CMD = @(
    "cmake",
    "-S", ".",
    "-B", $BUILD_DIR,
    "-G", "Visual Studio 18 2026",
    "-A", "x64"
    "-DCMAKE_BUILD_TYPE=$CMAKE_BUILD_TYPE"
)

if ($tidy)   { $CMAKE_CMD += "-DUSE_CLANG_TIDY=ON" }
if ($tests)  { $CMAKE_CMD += "-DBUILD_CAE_TESTS=ON" }
if ($format) { $CMAKE_CMD += "-DUSE_CLANG_FORMAT=ON" }
if ($doc)    { $CMAKE_CMD += "-DBUILD_DOC=ON" }

Write-Host "[INFO] Configuring project..."
& $CMAKE_CMD[0] $CMAKE_CMD[1..($CMAKE_CMD.Length - 1)]

if ($LASTEXITCODE -ne 0) {
    exit 1
}

$BUILD_TARGETS = @()

if (
(-not $tests -and -not $doc -and -not $format) -or $tidy
) {
    $BUILD_TARGETS += "all"
}

if ($format) { $BUILD_TARGETS += "cae-clang-format" }
if ($tests)  { $BUILD_TARGETS += "cae-tests" }
if ($doc)    { $BUILD_TARGETS += "doxygen" }

if ($BUILD_TARGETS.Count -eq 0) {
    Write-Host "[INFO] Nothing to build."
    exit 0
}

Write-Host "[INFO] Building targets: $($BUILD_TARGETS -join ' ')"

cmake --build $BUILD_DIR --target $BUILD_TARGETS

if ($LASTEXITCODE -ne 0) {
    exit 1
}

Write-Host "[INFO] Build completed successfully." -ForegroundColor Green
