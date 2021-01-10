/*Matthew Fritzz
 * CS 261
 * Project 2 Word Frequency*/
#include <stdio.h>
#include <string.h>
#define WORD_SIZE (50)
#define WORD_ARRAY_SIZE (300)

/*will be used to make an array of words*/
struct Freq{
   /* The word.  If word[0] is 0, this means this array entry is not used yet */
   char word[WORD_SIZE];
   /* The count of how many times this word is found */
   int count;
   /* Used to sort the words where 0 means first, 1 means second, ...
      and -1 means non sorted */
   int sortOrder;
};

/* initializes word array*/
void setupWordArray( struct Freq * wordArray )
{
   int i;
   for( i = 0; i < WORD_ARRAY_SIZE; i++ )
   {
      wordArray[i].word[0] = 0;
      wordArray[i].count = 0; 
      wordArray[i].sortOrder = -1;
   }
}

/* takes the word array and a word and checks if the word is in the array
if it is not, adds word, if is, increments counter of that word*/
void addWord( struct Freq * wordArray, char * word)
{
   int arrayPos = 0;

   while(wordArray[arrayPos].word[0] != 0) {
      if(strcmp(wordArray[arrayPos].word,word) == 0) {
         /* found word.. increment counter and return*/
         wordArray[arrayPos].count++;
         return;
      }
      arrayPos++;
   }

   /* this word not found, add it */
   strcpy(wordArray[arrayPos].word, word);
   wordArray[arrayPos].count = 1;
}

/* This sorts the word array in place.  Instead of moving things around, use the sortOrder
structure entry to indicate the sort order with 0 being the first value, 1 the second, and so on.
This function repeats the sort sequence until all words have been sorted.
A word is sorted if sortOrder != -1.  Every time through, the code finds the highest word/count.
Then that word's sortOrder is set to sortVal. sortVal then gets bigger by one and the loop repeats. */
void sortWordArray(struct Freq * wordArray )
{
   int sortVal = 0;

   while( 1 ) 
   {
      int biggest = 0;
      int arrayPos = 0;

      /* Find first non-sorted word */
      while( (wordArray[arrayPos].word[0] != 0) && 
             (wordArray[arrayPos].sortOrder >= 0) ) {
         arrayPos++;
      }

      /* None left */
      if( wordArray[arrayPos].word[0] == 0 )
      {
         return;
      }

      biggest = arrayPos;
      arrayPos++;

      /* Now loop through all remaining non-sorted words looking for the highest */
      while( wordArray[arrayPos].word[0] != 0)
      {
         /* If this word already sorted, go to next */
         if( wordArray[arrayPos].sortOrder >= 0)
         {
            arrayPos++;
            continue;
         }

         if( wordArray[arrayPos].count > wordArray[biggest].count )
         {
            /* If a word is found with a bigger count, move biggest to this word */
            biggest = arrayPos;
         }
         else if( (wordArray[arrayPos].count == wordArray[biggest].count ) &&
                  (strcmp(wordArray[arrayPos].word, wordArray[biggest].word) < 0) )
         {
            /* If a word is found with same count, but the word is less than biggest,
               move biggest to this word */
            biggest = arrayPos;
         }
         arrayPos++;
      }

      /* biggest now is the best remaining unsorted word.
         Set sortOrder to sortVal which makes it sorted */
      wordArray[biggest].sortOrder = sortVal;

      /* Increase sortVal ready for finding next word */
      sortVal++;
   }

}

/* prints top 5 counted words.  Just print each word where sortOrder is 0, then 1, and so on */
void printTop5( struct Freq * wordArray )
{
   int i;
   printf("\nCount\tWord\n=====\t====\n");
   for( i = 0; i < 5; i++ )
   {
      int arrayPos = 0;
      while(wordArray[arrayPos].word[0] != 0) {
         if( wordArray[arrayPos].sortOrder == i )
         {
             printf("%d\t%s\n", wordArray[arrayPos].count, wordArray[arrayPos].word);
             break;
         }
         arrayPos++;
      }
   }
}

/* Takes input string and get one word at a time using scanf.
   If it isn't the quit word, pass word to addWord function.
   After all words have been inserted, sort them and print them. */
int main()
{
   struct Freq wordArray[WORD_ARRAY_SIZE];
   char inputString[200];

   setupWordArray( &(wordArray[0]) );

   while( 1 )
   {
      scanf("%s", inputString);
      if( (inputString[0] < 'a') || (inputString[0] > 'z') )
      {
          continue;
      }

      if(strcmp(inputString,"zzzzz")==0)
      {
        break;
      }

      addWord(wordArray,inputString);
  }

  sortWordArray(&(wordArray[0]));
  printTop5(&(wordArray[0]));

}
