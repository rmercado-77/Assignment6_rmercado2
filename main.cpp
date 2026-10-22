#include "coordinate.h"

#include <cmath>
#include <exception>
#include <iomanip>
#include <iostream>
#include <random>
#include <string>

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0]
                  << " <number_of_coordinates>\n";
        return 1;
    }

    int number_of_coordinates;

    // Convert the command-line input and check that it is valid.
    try
    {
        std::string argument = argv[1];
        std::size_t characters_used = 0;

        number_of_coordinates =
            std::stoi(argument, &characters_used);

        if (characters_used != argument.size() ||
            number_of_coordinates <= 0)
        {
            std::cerr << "Enter a positive whole number.\n";
            return 1;
        }
    }
    catch (const std::exception &)
    {
        std::cerr << "Enter a valid positive integer.\n";
        return 1;
    }

    // The starter stays in place and does not count as a point.
    Coordinate starter{0.0f, 0.0f, 0, nullptr, nullptr};

    Coordinate *list_beginning = &starter;
    Coordinate *list_end = &starter;

    std::random_device seed;
    std::mt19937 generator(seed());
    std::uniform_real_distribution<float> distribution(0.0f, 100.0f);

    std::cout << std::fixed << std::setprecision(2);

    for (int i = 0; i < number_of_coordinates; ++i)
    {
        float x = distribution(generator);
        float y = distribution(generator);

        add_coordinate(list_end, x, y);
        list_end = list_end->next;
    }

    std::cout << "\nForward display:\n";
    forward_display(list_beginning);

    std::cout << "\nBackward display:\n";
    backward_display(list_end);

    std::cout << "\nList length: "
              << list_length(list_beginning) << '\n';

    std::cout << "\nClosest-point search:\n";
    closest_to(list_beginning, 50.0f, 50.0f);

    std::cout << "\nDeleting coordinate 1:\n";
    delete_coordinate(list_beginning, 1);

    // Find the end again in case the last point was deleted.
    list_end = list_beginning;

    while (list_end->next != nullptr)
    {
        list_end = list_end->next;
    }

    std::cout << "\nForward display after deletion:\n";
    forward_display(list_beginning);

    std::cout << "\nBackward display after deletion:\n";
    backward_display(list_end);

    std::cout << "\nNew list length: "
              << list_length(list_beginning) << '\n';

    // Free the remaining nodes, saving the next address first.
    Coordinate *current = list_beginning->next;

    while (current != nullptr)
    {
        Coordinate *next_coordinate = current->next;
        delete current;
        current = next_coordinate;
    }

    starter.next = nullptr;

    return 0;
}

int list_length(Coordinate *list_beginning)
{
    if (list_beginning == nullptr)
    {
        return 0;
    }

    int count = 0;

    // Skip the starter node since it is not a coordinate.
    Coordinate *current = list_beginning->next;

    while (current != nullptr)
    {
        ++count;
        current = current->next;
    }

    return count;
}

void add_coordinate(Coordinate *list_end, float x, float y)
{
    if (list_end == nullptr)
    {
        return;
    }

    Coordinate *beginning = list_end;

    // Go back to the start to get the list length.
    while (beginning->previous != nullptr)
    {
        beginning = beginning->previous;
    }

    Coordinate *new_coordinate = new Coordinate{
        x,
        y,
        list_length(beginning) + 1,
        nullptr,
        list_end
    };

    list_end->next = new_coordinate;
}

void forward_display(Coordinate *list_beginning)
{
    if (list_beginning == nullptr ||
        list_beginning->next == nullptr)
    {
        std::cout << "The list is empty.\n";
        return;
    }

    Coordinate *current = list_beginning->next;

    while (current != nullptr)
    {
        std::cout << "ID " << current->coord_id
                  << ": (" << current->x
                  << ", " << current->y << ")\n";

        current = current->next;
    }
}

void backward_display(Coordinate *list_end)
{
    if (list_end == nullptr || list_end->previous == nullptr)
    {
        std::cout << "The list is empty.\n";
        return;
    }

    Coordinate *current = list_end;

    // Stop before displaying the starter node.
    while (current->previous != nullptr)
    {
        std::cout << "ID " << current->coord_id
                  << ": (" << current->x
                  << ", " << current->y << ")\n";

        current = current->previous;
    }
}

void delete_coordinate(Coordinate *list_beginning,
                       int coord_id_to_delete)
{
    if (list_beginning == nullptr)
    {
        return;
    }

    Coordinate *current = list_beginning->next;

    while (current != nullptr &&
           current->coord_id != coord_id_to_delete)
    {
        current = current->next;
    }

    if (current == nullptr)
    {
        std::cout << "Coordinate ID "
                  << coord_id_to_delete
                  << " was not found.\n";
        return;
    }

    // Connect the surrounding nodes before deleting this one.
    current->previous->next = current->next;

    if (current->next != nullptr)
    {
        current->next->previous = current->previous;
    }

    delete current;

    // Renumber IDs so the next added ID can use length + 1.
    int id = 1;
    current = list_beginning->next;

    while (current != nullptr)
    {
        current->coord_id = id++;
        current = current->next;
    }

    std::cout << "Deleted coordinate ID "
              << coord_id_to_delete << ".\n";
}

void closest_to(Coordinate *list_beginning, float x, float y)
{
    if (list_beginning == nullptr ||
        list_beginning->next == nullptr)
    {
        std::cout << "The list is empty. No closest point.\n";
        return;
    }

    // Use the first point as the starting closest point.
    Coordinate *closest = list_beginning->next;

    double dx = static_cast<double>(closest->x) - x;
    double dy = static_cast<double>(closest->y) - y;
    double smallest_distance_squared = dx * dx + dy * dy;

    Coordinate *current = closest->next;

    while (current != nullptr)
    {
        dx = static_cast<double>(current->x) - x;
        dy = static_cast<double>(current->y) - y;

        double distance_squared = dx * dx + dy * dy;

        if (distance_squared < smallest_distance_squared)
        {
            smallest_distance_squared = distance_squared;
            closest = current;
        }

        current = current->next;
    }

    // Only take the square root when printing the final distance.
    std::cout << "Closest point to (" << x << ", " << y << "):\n"
              << "ID " << closest->coord_id
              << ": (" << closest->x
              << ", " << closest->y << ")\n"
              << "Distance: "
              << std::sqrt(smallest_distance_squared) << '\n';
}
