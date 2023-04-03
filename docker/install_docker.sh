#!/bin/bash

# This script installs the necessary dependencies to run Docker on a Debian-based Linux system.

# Update the package index.
sudo apt-get update

# Install the necessary packages to allow apt to use a repository over HTTPS.
sudo apt-get install -y apt-transport-https ca-certificates curl gnupg lsb-release

# Add Docker’s official GPG key.
curl -fsSL https://download.docker.com/linux/ubuntu/gpg | sudo gpg --dearmor -o /usr/share/keyrings/docker-archive-keyring.gpg

# Set up the stable repository.
echo "deb [arch=amd64 signed-by=/usr/share/keyrings/docker-archive-keyring.gpg] https://download.docker.com/linux/ubuntu $(lsb_release -cs) stable" | sudo tee /etc/apt/sources.list.d/docker.list > /dev/null

# Update the package index again.
sudo apt-get update

# Install Docker.
sudo apt-get install docker-ce docker-ce-cli containerd.io docker-buildx-plugin docker-compose-plugin
