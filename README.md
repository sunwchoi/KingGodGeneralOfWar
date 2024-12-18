[언리얼포트폴리오.pdf](https://github.com/user-attachments/files/18180192/default.pdf)

# 갓 오브 워: 라그나로크 언리얼 모작프로젝트   
개발기간: 24.7 ~ 24.8   
팀 구성: 언리얼3, 아트1, 기획1    
담당: 보스ai

## 플레이 영상
https://github.com/user-attachments/assets/bc8c89a4-2c7b-4db5-a437-1b8924f574ba   

## 공격 부분 구현

**랜덤 공격패턴 출력**    
플레이어와의 거리마다 쓸 수 있는 공격을 next_states 배열에 넣어놓습니다.
rand함수를 통해 랜덤 인덱스를 넣어 패턴을 출력합니다.   

```c++
    float dist = FVector::Dist(myLoc, targetLoc);
    TArray<EAwakenThorState> NextStates;
    
    if (dist <= 200.f)
    {
        NextStates.Add(EAwakenThorState::MeleeAttackChange);
    }
    else if (200.f < dist && dist <= 700.f)
    {
        NextStates.Add(EAwakenThorState::RangedAttackChange);
        NextStates.Add(EAwakenThorState::BackTeleport);
        NextStates.Add(EAwakenThorState::BackTeleport);
        NextStates.Add(EAwakenThorState::LeftTeleport);
        NextStates.Add(EAwakenThorState::RightTeleport);
    }
    else
    {
        NextStates.Add(EAwakenThorState::RangedAttackChange);
        NextStates.Add(EAwakenThorState::Dash);
        NextStates.Add(EAwakenThorState::Dash);
        NextStates.Add(EAwakenThorState::Dash);
        NextStates.Add(EAwakenThorState::Dash);
    }
    
    int32 idx = FMath::RandRange(0, NextStates.Num() - 1);
    State = NextStates[idx];
```

**애니메이션 동기화**
fsm state에 맞는 애니메이션을 출력해야했습니다.   
처음에는 anim instance의 state머신을 사용했습니다. fsm 컴포넌트의 state에 따라 state머신의 state가 전환되도록 구현했습니다.   
    
*변경 전 state머신*
![image](https://github.com/user-attachments/assets/761adedc-670f-4074-b792-33db8bb2ea2f)

state가 추가될수록 anim instance의 구조가 복잡해지고 상태 전환마다의 조건을 계속 지정해 줘야하는 불편함이 있었습니다.
그래서 anim montage를 사용하는 방식으로 바꾸고 fsm컴포넌트에서 상태를 전환한 후 바로 애니메이션을 재생하는 방식을 사용했습니다.

*변경 후 state머신*    
![image](https://github.com/user-attachments/assets/d2d96614-845e-435d-b97b-a5c8ae073dec)

*애니메이션 몽타주*
![image](https://github.com/user-attachments/assets/318b0f95-395f-47c0-958d-e3c0fef6eb4e)

*fsm컴포넌트의 몽타주 실행코드*
```c++
void UAwakenThorFSM::JumpAttackState()
{
	Anim->PlayJumpAttackMontage();
}

void UAwakenThorFSM::PoundAttackState()
{
	Anim->PlayPoundAttackMontage();
}

void UAwakenThorFSM::KickAttackState()
{
	Anim->PlayKickAttackMontage();
}

void UAwakenThorFSM::ClapAttackState()
{
	Anim->PlayClapAttackMontage();
}
```



