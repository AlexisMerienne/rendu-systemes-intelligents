# Use an existing image as the base image
FROM python:3.8
#install numpy
RUN pip install numpy
# Copy the client script to the container
COPY braitenberg.py /app/

# Set the working directory
WORKDIR /app/

# Run the client script
CMD [ "python", "braitenberg.py" ]