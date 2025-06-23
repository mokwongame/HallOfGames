public enum EndReason
{
    None,
    KilledByStone,
    ScoreReached
}

public static class GameResult
{
    public static EndReason endReason = EndReason.None;
}

