#include <chaos/vote.h>
#include <new>

namespace chaos {

    Vote* gpVote;


    void voteInit() {
        // Initalize the vote structure at the given address
        gpVote = new ((void*)VOTE_ADDRESS) Vote();
    }

    void voteShutdown() {
        gpVote->~Vote();
        gpVote = nil(Vote*);
    }

#define kSecond (u32)(60*1000)

    /// Starts a new vote with random effect choices.
    void voteCreateRandom() {
        // Guess a random tick time
        //gpVote->nrTicksLeft = 10 * kSecond + (rand32() * 500 % 25000);

#ifdef NO_CHAT_VOTING
        // TODO: Give some random results
#endif
    }

    /// Called at tick rate to update the vote.
    void voteUpdate() {
        if(gpVote->nrTicksLeft-- == 0) {
            // Find the winning vote, or tie(s), and enable them.

            // getEffectManager().enableEffect(...);


            // Create a new random vote
            voteCreateRandom();
        }
    }
}
