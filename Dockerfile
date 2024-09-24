FROM ubuntu:20.04

# Install necessary packages
RUN apt-get update && \
    apt-get install -y g++ libncurses5-dev libncursesw5-dev && \
    apt-get clean && \
    rm -rf /var/lib/apt/lists/*

# Set the working directory
WORKDIR /usr/src/app

# Copy the source code
COPY . .

# Compile your code (replace "main.cpp" with your actual file name)
RUN g++ -o arcanoid main.cpp -lncurses

# Command to run your program
CMD ["./arcanoid"]
