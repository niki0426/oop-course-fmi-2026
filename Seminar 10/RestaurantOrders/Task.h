#pragma once

#include <cstddef>
#include <iosfwd>
#include <memory>
#include <string>
#include <vector>

class Task
{
private:
    std::string title;
    std::string description;
    bool completed = false;
    std::vector<std::unique_ptr<Task>> subtasks;

    void print(std::ostream& os, std::size_t indent) const;

public:
    Task(const std::string& title, const std::string& description);

    Task(const Task&) = delete;
    Task& operator=(const Task&) = delete;

    Task(Task&&) noexcept = default;
    Task& operator=(Task&&) noexcept = default;

    const std::string& getTitle() const;
    const std::string& getDescription() const;
    bool isCompleted() const;

    void markCompleted();
    void markIncomplete();

    bool addSubtask(const std::string& title, const std::string& description);
    bool removeSubtask(const std::string& title);

    Task& findSubtask(const std::string& title);
    const Task& findSubtask(const std::string& title) const;

    std::size_t getSubtaskCount() const;
    std::size_t getTotalTaskCount() const;
    bool allSubtasksCompleted() const;

    friend std::ostream& operator<<(std::ostream& os, const Task& task);
};
