#include "../includes/retriever.hpp"
#include <iostream>

int main() {
  std::vector<std::string> ingredients = {"eggs", "flour"};
  Retriever testing_one = Retriever(ingredients, 6);
  for (int i = 0; i < 6; i++) {
      std::cout << testing_one.GetLinks()[i] << std::endl;
  }
}