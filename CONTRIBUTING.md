# Contributing

Thanks for considering contributing to system-service. You are more than welcome!.

## How you can contribute

### Opening Issues

If you found a bug, you think something can be improved or you realize a new feature can be introduced in the project, please, feel free to open an issue and fill it with as some information as possible. 

### Posting Pull Requests

  - Follow our code style, otherwhise your pull request will be refused and you will need to work on it.
  - Please make sure to test your changes before submitting your pull request.
  - Create a pull request only for fixing **one** bug, introducing **one** new feature or **one** improvement.
  - A pull request **shall** be linked to an issue.
  - Please, mention the issue number in the commit of your pull request, in order to have issue traceability, e.g: "#9 fix blablabla in foo function".

## Development Guidelines

If you are not familiar with a workflow you can start reading this: [Understanding the GitHub flow](https://guides.github.com/introduction/flow/).

1. Fork this repository and clone it to your computer. If you need some help: [Forking Projects](https://guides.github.com/activities/forking/).
2. If you have forked and cloned the project before and it has been a while since you worked on it:
   ```
   git remote add upstream https://github.com/maherme/system-service.git
   git fetch upstream
   git checkout master
   git merge upstream/master
   git add .
   git commit -m "your comments"
   git push
   ```
4. Make your changes:
   - Write your code (please, we have a code style guidelines, you should follow it).
   - **Test your code**.
   - Document your code, (we use doxygen format, please, take this into account).
5. Commit and push your changes.
6. Submit your pull request.
