#ifndef CHAOS_VOTE_BLOB_HPP
#define CHAOS_VOTE_BLOB_HPP

namespace chaos {

    void voteInit();
    void voteShutdown();

    /// Starts a new vote with random effect choices.
    void voteCreateRandom();

    /// Called at tick rate to update the vote.
    void voteUpdate();
}

#endif
