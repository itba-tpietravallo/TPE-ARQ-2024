# Validates the existance of the TPE-ARQ container, starts it up & compiles the project
CONTAINER_NAME="TPE-ARQ"

# COLORS
RED='\033[0;31m'
YELLOW='\033[1;33m'
GREEN='\033[0;32m'
NC='\033[0m'

docker ps -a &> /dev/null

if [ $? -ne 0 ]; then
    echo "${RED}Docker is not running. Please start Docker and try again.${NC}"
    exit 1
fi

# Check if container exists
if [ ! "$(docker ps -a | grep "$CONTAINER_NAME")" ]; then
    echo "${YELLOW}Container $CONTAINER_NAME does not exist. ${NC}"
    echo "Pulling image..."
    docker pull agodio/itba-so:2.0
    echo "Creating container..."
    # Note: ${PWD}:/root. Using another container to compile might fail as the compiled files would not be guaranteed to be at $PWD
    # Always use TPE-ARQ to compile
    docker run -d -v ${PWD}:/root --security-opt seccomp:unconfined -it --name "$CONTAINER_NAME" agodio/itba-so:2.0
    echo "${GREEN}Container $CONTAINER_NAME created.${NC}"
else
    echo "${GREEN}Container $CONTAINER_NAME exists.${NC}"
fi

# Start container
docker start "$CONTAINER_NAME" &> /dev/null
echo "${GREEN}Container $CONTAINER_NAME started.${NC}"

# Compiles
docker exec -it "$CONTAINER_NAME" make clean -C /root/ && \
docker exec -it "$CONTAINER_NAME" make all -C /root/Toolchain && \
docker exec -it "$CONTAINER_NAME" make all -C /root/

# If an argument "PERF_COMPARE" is supplied, then compare setting the performance flag
if [ "$1" == "PERF_COMPARE" ]; then
    docker exec -it "$CONTAINER_NAME" make clean -C /root/Kernel && \
    docker exec -it "$CONTAINER_NAME" make all -C /root/ NO_OPTIMIZATIONS=1 OSIMGNAME="x64BareBonesImage_no_optimizations"
fi

if [ $? -ne 0 ]; then
    echo "${RED}Compilation failed.${NC}"
    exit 1
fi

echo "${GREEN}Compilation finished.${NC}"
