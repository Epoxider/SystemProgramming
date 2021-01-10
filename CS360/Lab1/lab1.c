struct Person {
  char name[20];
  bool isMarried;
  struct *marriedTo;
  struct *divorcedTo;
  struct *child;

};

int main() {

  struct Person person1, person2;
  return 0;

}
  
